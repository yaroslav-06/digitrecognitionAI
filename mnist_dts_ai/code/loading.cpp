#include <iostream>
#include <iomanip>
#include "loader.h"

void Loader::out_ln(int cr){
    std::cout << cr << " / " << mx << " [";
    const int line_len = 40;
    for(int i = 0; i < line_len; ++i)
        std::cout << ((cr - mn) * 100 / (mx - mn) >= i * 100 / line_len ?'#' :' ');
    std::cout << "] " << std::setprecision(2) << (cr - mn) * 100 / (mx - mn) << '%';
}
void Loader::delete_line(){
    std::cout << "\33[2K\r";
}
Loader::Loader(int mnv, int mxv){
    mn = mnv; mx = mxv;
    out_ln(mn);
}
void Loader::update(int cr){
    delete_line();
    out_ln(cr);
    std::cout.flush();
}
