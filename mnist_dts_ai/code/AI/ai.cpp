#include <fstream>
#include <cmath>
#include "ai.h"

float ai::mistake(float* real_ans){
    float ans = 0;
    for(int i = 0; i < sz[n - 1]; i++){
        ans += pow(abs(mr[n - 1][i] - real_ans[i]), 1.5);
    }
    return ans;
}
