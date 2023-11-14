#include <fstream>
#include "ai.h"

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
