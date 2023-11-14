#include <fstream>
#include <cmath>
#include "ai.h"

float sigmoid(float n){
    return 1 / (1 + pow(M_E, -n));
}

float relu_leaky(float n){
    //return sigmoid(n);
    if(n >= 0) return n;
    return n/leakyDiv;
}

float derRelu_leaky(float n){
    //return sigmoid(n) * (1 - sigmoid(n));
    if(n >= 0) return 1;
    return leakyDiv;
}

ai::ai(const ai &original){
    save_to = original.save_to;
    n = original.n;
    sz = new int[n];
    std::copy(original.sz, original.sz + n, sz);
    biases = new float*[n];
    bsm = new float*[n];
    mr = new float*[n];
    mods = new float*[n];
    weights = new float**[n - 1];
    wsm = new float**[n - 1];
    for(int i = 0; i < n; i++)
        mods[i] = new float[sz[i]];
    for(int i = 0; i < n - 1; i++){
        weights[i] = new float  *[sz[i]];
        wsm[i] = new float*[sz[i]];
        mr[i] = new float[sz[i]];
        biases[i] = new float[sz[i]];
        bsm[i] = new float[sz[i]];
        std::copy(original.biases[i], original.biases[i] + sz[i], biases[i]);
        for(int j = 0; j < sz[i]; j++){
            weights[i][j] = new float[sz[i + 1]];
            wsm[i][j] = new float[sz[i + 1]];
            std::copy(original.weights[i][j], original.weights[i][j] + sz[i + 1], weights[i][j]);
        }
    }
    biases[n - 1] = new float[sz[n - 1]];
    bsm[n - 1] = new float[sz[n - 1]];
    mr[n - 1] = new float[sz[n - 1]];
    mods[n - 1] = new float[sz[n - 1]];
    std::copy(original.biases[n - 1], original.biases[n - 1] + sz[n - 1], biases[n - 1]);
}

ai::ai(int intstr[], int sz_len, std::string save_dir, bool writen){
    std::fstream save;
    save_to = save_dir;
    save.open(save_dir);
    n = sz_len;
    sz = new int[n];
    std::copy(intstr, intstr + n, sz);
    biases = new float*[n];
    bsm = new float*[n];
    mr = new float*[n];
    mods = new float*[n];
    weights = new float**[n - 1];
    wsm = new float**[n - 1];
    for(int i = 0; i < n; i++)
        mods[i] = new float[sz[i]];
    for(int i = 0; i < n - 1; i++){
        weights[i] = new float  *[sz[i]];
        wsm[i] = new float*[sz[i]];
        mr[i] = new float[sz[i]];
        biases[i] = new float[sz[i]];
        bsm[i] = new float[sz[i]];
        for(int j = 0; j < sz[i]; j++){
            if(writen)
                save.read(reinterpret_cast<char*>(biases[i] + j), sizeof(biases[i][j]));
            else biases[i][j] = (rand() % 256 - 128) / 256.0;
            weights[i][j] = new float[sz[i + 1]];
            wsm[i][j] = new float[sz[i + 1]];
            for(int k = 0; k < sz[i + 1]; k++){
                if(writen)
                    save.read(reinterpret_cast<char*> (weights[i][j] + k), sizeof(weights[i][j][k]));
                else weights[i][j][k] = (rand() % 256 - 128) / 256.0;
            }
        }
    }
    biases[n - 1] = new float[sz[n - 1]];
    bsm[n - 1] = new float[sz[n - 1]];
    mr[n - 1] = new float[sz[n - 1]];
    mods[n - 1] = new float[sz[n - 1]];
    for(int i = 0; i < sz[n - 1]; i++){
        if(writen)
            save.read(reinterpret_cast<char*> (biases[n - 1] + i), sizeof(biases[n - 1][i]));
        else biases[n - 1][i] = (rand() % 256 - 128) / 256.0;
    }
    save.close();
}

void ai::save_ai(){
    // saving ai indeses
    std::ofstream save;
    save.open(save_to, std::ios::binary);
    save.clear();
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < sz[i]; j++){
            save.write(reinterpret_cast<char*> (biases[i] + j), sizeof(biases[i][j]));
            for(int k = 0; k < sz[i + 1]; k++)
                save.write(reinterpret_cast<char*> (weights[i][j] + k), sizeof(weights[i][j][k]));
        }
    }
    for(int i = 0; i < sz[n - 1]; i++)
        save.write(reinterpret_cast<char*> (biases[n - 1] + i), sizeof(biases[n - 1][i]));
    save.close();
}

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
float ai::mistake(float* real_ans){
    float ans = 0;
    for(int i = 0; i < sz[n - 1]; i++){
        ans = ans + pow(abs(mr[n - 1][i] - real_ans[i]), 1.5);
    }
    return ans;
}
