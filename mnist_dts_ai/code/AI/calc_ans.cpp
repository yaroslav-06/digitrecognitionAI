#include "ai.h"

float* ai::calc_ans(float *inp){
    //set inp to midreses
    for(int i = 0; i < sz[0]; i++)
        mr[0][i] = inp[i] + biases[0][i];

    for(int i = 1; i < n; i++){
        for(int k = 0; k < sz[i]; k++){
            mr[i][k] = biases[i][k];
            for(int j = 0; j < sz[i - 1]; j++)
                mr[i][k] += mr[i - 1][j] * weights[i - 1][j][k];
            mr[i][k] = relu_leaky(mr[i][k]);
        }
    }
    return mr[n - 1];
}
