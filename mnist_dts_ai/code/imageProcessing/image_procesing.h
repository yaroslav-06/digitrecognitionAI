#include "../AI/mh.h"
#include "pI.h"


void convolution(ai_dt &ps, shp cnn_sz, int filters);

void max_pooling(ai_dt &ps, shp sz);

ai_dt pics_to_data(pics &ps);

void out_dts(dts &dt, shp sz);

enum PictureSet {TestPictureSet, TrainingPictureSet};

class Layer{
    public:
    virtual void run(dts &dt);
    virtual void print_type();
};

class Convolution: public Layer{
    int fs, dots;
    float **filt;
    shp pht_sz, this_sz;
    public:
    Convolution(shp, shp&, int);
    void run(dts &dt)override;
    void print_type()override;
};

class MaxPool: public Layer{
    int dots;
    shp this_sz, pht_sz;
    public:
        MaxPool(shp, shp&);
        void run(dts &dt)override;
        void print_type()override;
};

struct LayerNode{
    Layer *layer;
    LayerNode *next;
};

class ImageFilters{
    LayerNode *start, *end;
    public:
    ImageFilters(){
        start = 0;
        end = 0;
    }
    void add(Layer *layer);
    void process(ai_dt &im);
};
