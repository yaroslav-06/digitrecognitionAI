#include <cmath>
#include "ai.h"

float sigmoid(float n){
    return 1 / (1 + pow(M_E, -n));
}

float derSigmoid(float n){
    return sigmoid(n) * (1 - sigmoid(n));
}

float relu_leaky(float n){
    if(n >= 0) return n;
    return n/leakyDiv;
}

float derRelu_leaky(float n){
    if(n >= 0) return 1;
    return leakyDiv;
}
