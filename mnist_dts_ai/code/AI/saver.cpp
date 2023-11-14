#include <fstream>
#include "ai.h"

void ai::save_ai(){
    // saving ai indices
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
