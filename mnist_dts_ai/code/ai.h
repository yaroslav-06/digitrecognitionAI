#include <iostream>

const int leakyDiv = 100;

int relu_leaky(int n);
char derRelu_leaky(int n);

enum DefaultAI {NewRandomAI, LastSavedAI};

class ai{
    float ***weights, **biases, **mr, ***wsm, **bsm, **mods;
    int *sz, n;
    std::string save_to;
    public:
    ai(int intstr[], int sz_len, std::string save_dir, bool writen);
    ai(const ai &original);
    void save_ai();

    float* calc_ans(float *inp);

    void backpropogation_set(float *real_ans);
    void backpropagation_save(int cases);
    float mistake(float* real_ans);
};
