#include <iostream>
#include <iomanip>
#include "loader.h"
#include "image_procesing.h"
#include "ai.h"

void pic_to_dts(pic &ps, dts &dt, shp sz){
    dt.bt = new float[sz.h * sz.w];
    for(int i = 0; i < sz.h * sz.w; ++i)
        dt.bt[i] = ((ps.pix[i] + 256) % 256) / 256.0;
    dt.ans = ps.ans;
}

ai_dt pics_to_data(pics &ps){
    ai_dt res;
    res.len = ps.len;
    res.dt = new dts[ps.len];
    res.sz = {ps.photos->rs, ps.photos->cs};
    for(int i = 0; i < res.len; i++) 
        pic_to_dts(ps.photos[i], res.dt[i], res.sz);
    return res;
}

int from_coord(shp sz, int x, int y){
    return x + y * sz.w;
}
// CONVOLUTION

void convolution(dts &dt, float **filter, shp cnn_sz, shp dt_sz, int n, int dots){
    float *nbt = new float[dots * n];
    int cr = -1;
    float *f;
    for(int fsc = 0; fsc < n; ++fsc){
        f = filter[fsc];
        for(int l = 0; l < dt_sz.l; ++l)
        for(int i = 0; i < dt_sz.h + 1 - cnn_sz.h; ++i)
            for(int j = 0; j < dt_sz.w + 1 - cnn_sz.w; ++j){
                float rs = 0;
                for(int h = 0; h < cnn_sz.h; ++h)
                    for(int w = 0; w < cnn_sz.w; ++w){
                         rs += (dt.bt[from_coord(dt_sz, j + w, i + h)] * 2 - 1.f) * 
                                         f[from_coord(cnn_sz, w, h)]; 
                    }
                rs = rs / (cnn_sz.h * cnn_sz.w * 2);
                nbt[++cr] = rs;
            }
    }
    delete[] dt.bt;
    dt.bt = nbt;
}

//MAX POOLING

void max_pooling(dts &dt, shp dt_sz, shp mp_sz, int af_ln){
    float *nbt = new float[af_ln];
    float cr;
    int ind = -1;
    for(int l = 0; l < dt_sz.l; l++)
        for(int i = 0; i < dt_sz.h; i += mp_sz.h)
            for(int j = 0; j < dt_sz.w; j += mp_sz.w){
                cr = 0;
                for(int h = 0; h < mp_sz.h; ++h)
                    for(int w = 0; w < mp_sz.w; ++w)
                        cr = std::max(cr, dt.bt[from_coord(dt_sz, j + w, i + h)]);
                ++ind;
                nbt[ind] = cr;
            }
    delete[] dt.bt;
    dt.bt = nbt;
}


void Layer::run(dts &dt){
    std::cout << "running empty layer: " << dt.ans << '\n';
}

void Layer::print_type(){
    std::cout << "this is empty layer\n";
}

float** gen_filter(int n, shp sz){
    float **filter = new float*[n];
    for(int i = 0; i < n; i++){
        filter[i] = new float[sz.h * sz.w];
        for(int j = 0; j < sz.h * sz.w; ++j)
            filter[i][j] = (rand() % 513 - 256) / 256.f;
    }
    return filter;
}

Convolution::Convolution(shp cnn_sz, shp &photo_sz, int filters){
    filt = gen_filter(filters, cnn_sz);
    fs = filters;
    this_sz = cnn_sz;
    pht_sz = photo_sz;
    photo_sz = {pht_sz.w + 1 - cnn_sz.w, pht_sz.h + 1 - cnn_sz.h, pht_sz.l * filters};
    dots = photo_sz.h * photo_sz.w * pht_sz.l;
}

void Convolution::run(dts &dt){
    convolution(dt, filt, this_sz, pht_sz, fs, dots);
}

void Convolution::print_type(){
    std::cout << "convolution layer\n"
         << "filters: " << fs << '\n'
         << "sz: " << this_sz.w << ", " << this_sz.h << "\n\n";
}


MaxPool::MaxPool(shp mp_sz, shp &photo_sz){
    this_sz = mp_sz;
    photo_sz = {(photo_sz.w + mp_sz.w - 1) / mp_sz.w, (photo_sz.h + mp_sz.h - 1) / mp_sz.h, photo_sz.l};
    pht_sz = photo_sz;
    dots = photo_sz.h * photo_sz.w * photo_sz.l;
}

void MaxPool::run(dts &dt){
    max_pooling(dt, pht_sz, this_sz, dots);
}

void MaxPool::print_type(){
    std::cout << "max pooling layer\n"
         << "sz: " << this_sz.w << ", " << this_sz.h << "\n\n";
}

void ImageFilters::process(ai_dt &im){
    LayerNode *cur;
    int percent_len = 20;
    Loader loader(1, im.len);
    for(int i = 0; i < im.len; i++){
        loader.update(i + 1);
        cur = start;
        while(cur != 0){
            cur->layer->run(im.dt[i]);
            cur = cur->next;
        }
    }
    std::cout << std::endl;
}
void ImageFilters::add(Layer *layer){
    if(end == 0)
        start = end = new LayerNode{layer, 0};
    else
        end = (end->next = new LayerNode{layer, 0});
}
