#include "ai.h"

void ai::backpropogation_set(float *real_ans){
    //int beauty_number = 300; // is number that's just what I think is the best to modify ai by step
    //Setting changes to output layer
    for(int i = 0; i < sz[n - 1]; i++)
        mods[n - 1][i] = 2 * (real_ans[i] - mr[n - 1][i]) / derRelu_leaky(mr[n - 1][i]),
        bsm[n - 1][i] += mods[n - 1][i];
    for(int i = n - 2; i >= 0; i--){
        for(int j = 0; j < sz[i]; j++){
            mods[i][j] = 0;
            for(int k = 0; k < sz[i + 1]; k++){
                wsm[i][j][k] += mr[i][j] * mods[i + 1][k] / derRelu_leaky(mr[i][j]);
                mods[i][j] += weights[i][j][k] * mods[i + 1][k] / derRelu_leaky(mr[i][j]);
                bsm[i][j] += mods[i + 1][k] / derRelu_leaky(mr[i][j]);
            }
        }
    }
}

void ai::backpropagation_save(int cases){
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < sz[i]; j++){
            biases[i][j] += bsm[i][j] / cases;
            bsm[i][j] = 0;
            for(int k = 0; k < sz[i + 1]; k++)
                if(wsm[i][j][k] / cases != 0)
                    weights[i][j][k] += wsm[i][j][k] / cases,
                    wsm[i][j][k] = 0;
        }
    }
    for(int i = 0; i < sz[n - 1]; i++)
        biases[n - 1][i] = bsm[n - 1][i] / cases,
        bsm[n - 1][i] = 0;
}
