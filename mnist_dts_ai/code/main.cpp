#include <iostream>
#include <iomanip>
#include "imageProcessing/image_procesing.h"
#include "ai/ai.h"

int train_ai(ai &ir, ai_dt &ps, int ans_sz, int cases, int j = 0, int save_divider = 333);

int curve_search_for_best(ai ir, ai_dt &pc, int sz[], int n, int cases){
    //find divider for pushing updates to ai that gives the best result in first run
    int l = 1, r = 0;
    for(int i = 0; i < n; ++i)
        r += sz[i];
    while(r - l >= 3){
        ai ir1 = ir,
           ir2 = ir;
        int m1 = l + (r - l) / 3, m2 = r - (r - l) / 3;
        if(train_ai(ir1, pc, sz[n - 1], cases, 0, m1) >
           train_ai(ir2, pc, sz[n - 1], cases, 0, m2))
            r = m2;
        else l = m1;
        std::cout << "search on [" << l << ".." << r << "]\n";
    }
    std::cout << "optimal: " << ((l + r) >> 1) << '\n';
    return (l + r) >> 1;
}

int main(){
    pics ps = readPict(TrainingPictureSet);
    ai_dt pc = pics_to_data(ps);
    ImageFilters img_edit;
    //img_edit.add(new Convolution({3, 3}, pc.sz, 6));
    //img_edit.add(new MaxPool({2, 2}, pc.sz));
    std::cout << "processing photo filters:\n";
    img_edit.process(pc);
    int n = 4, sz[] = {pc.sz.w * pc.sz.h * pc.sz.l, 40, 40, 10};
    ai ir(sz, n, "../saved_ai/ai", NewRandomAI);
    int tst = 5, correct = 0, cases = 100;
    std::cout << "training ai:\n";
    //std::cout << curve_search_for_best(ir, pc, sz, n, cases) << '\n';
    for(int j = 0; j < tst; j++)
        correct += train_ai(ir, pc, sz[n - 1], cases, j);
    std::cout << "correct results: " << correct << " / " << pc.len * tst
         << " is " << std::setprecision(1) << std::fixed << correct * 100.0 / (pc.len*tst) << "%" << '\n';
}
