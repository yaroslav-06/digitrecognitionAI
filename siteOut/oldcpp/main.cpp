#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

typedef long long ll;

int pLen;

int max_el(int* a, int n){
    int ans = 0;
    for(int i = 1; i < n; i++)
        if(a[ans] < a[i]) ans = i;
    return ans;
}

int* generate_ans(int ans, short len){
    int *a = new int[len];
    a[ans] = 255;
    return a;
}

int l_sum(int* l, int len){
    int ans;
    for(int i=0; i<len; i++)
        ans += l[i];
    return ans;
}

const int leakyDiv = 100;

int relu_l(int n){
    if(n >= 0) return n;
    return n/leakyDiv;
}

char derRelu_l(int n){
    if(n >= 0) return 1;
    else return leakyDiv;
}

//// Prossec image
struct pI{
    char ans;
    unsigned char rs, cs;
    char pix[784];
};

pI* readPict(){
    fstream ansI, pixI;
    ansI.open("siteOut/cpp/train-labels");
    pixI.open("siteOut/cpp/train-images");
    unsigned char* vals = new unsigned char[4];
    uint32_t headers[4]; // Magic NUM | N (ROWs | COLs)
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 4; j++)
            ansI >> vals[j],
            headers[i] = headers[i] + (vals[j] << (8 * (3 - j)));
    
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            pixI >> vals[j],
            headers[i] = (headers[i] << 8) + vals[j];
    cout << "magic N: " << headers[0] << '\n'
        << "N: "  << headers[1] << '\n'
        << "rs: " << headers[2] << '\n'
        << "cs: " << headers[3] << '\n';
    delete[] vals;
    pI* photos = new pI[headers[1]];
    for(int i = 0; i < headers[1]; i++){
        ansI.read( &(photos[i].ans), 1);
        photos[i].rs = headers[2];
        photos[i].cs = headers[3];
    }
    for(int j = 0; j < headers[1]; j++){
        for(int row = 0; row < headers[2]; row++)
            for(int col = 0; col < headers[3]; col++)
                pixI.read(&photos[j].pix[row * photos[j].rs + col], 1);
    }
    pLen = headers[1];
    return photos;
}

class ai{
    int ***weights, **biases, *sz, n, **mr, ***wsm, **bsm, **mods;

    public:
    //initialization of ws and bs
    ai(int intstr[], int sz_len){
        n = sz_len;
        sz = intstr;
        biases = new int*[n];
        bsm = new int*[n];
        mr = new int*[n];
        mods = new int*[n];
        weights = new int**[n - 1];
        wsm = new int**[n - 1];
        for(int i = 0; i < n; i++)
            mods[i] = new int[sz[i]];
        for(int i = 0; i < n - 1; i++){
            weights[i] = new int*[sz[i]];
            wsm[i] = new int*[sz[i]];
            mr[i] = new int[sz[i]];
            biases[i] = new int[sz[i]];
            bsm[i] = new int[sz[i]];
            for(int j = 0; j < sz[i]; j++){
                biases[i][j] = rand() % 256 - 128;
                weights[i][j] = new int[sz[i + 1]];
                wsm[i][j] = new int[sz[i + 1]];
                for(int k = 0; k < sz[i + 1]; k++)
                    weights[i][j][k] = rand() % 256 - 128;
            }
        }
        biases[n - 1] = new int[sz[n - 1]];
        bsm[n - 1] = new int[sz[n - 1]];
        mr[n - 1] = new int[sz[n - 1]];
        mods[n - 1] = new int[sz[n - 1]];
        for(int i = 0; i < sz[n - 1]; i++)
            biases[n - 1][i] = rand() % 256 - 128;
    }

    int* calc_ans(char inp[]){
        //set inp to midreses
        for(int i = 0; i < sz[0]; i++)
            mr[0][i] = (inp[i] + 256) % 256 + biases[0][i];

        for(int i = 1; i < n; i++){
            for(int k = 0; k < sz[i]; k++){
                mr[i][k] = biases[i][k];
                for(int j = 0; j < sz[i - 1]; j++)
                    mr[i][k] += mr[i - 1][j] * weights[i - 1][j][k] / 256;
                mr[i][k] = relu_l(mr[i][k]);
            }
        }
        return mr[n - 1];
    }

    void backpropogation_set(int *real_ans){
        //Setting needed changes to output layer
        for(int i = 0; i < sz[n - 1]; i++)
            mods[n - 1][i] = 2 * (real_ans[i] - mr[n - 1][i]) / derRelu_l(mr[n - 1][i]),
            bsm[n - 1][i] += mods[n - 1][i],
            cout << i << ") " << mods[n - 1][i] << '\t';
        cout << "last modify\n";
        for(int i = n - 2; i >= 0; i--){
            for(int j = 0; j < sz[i]; j++){
                mods[i][j] = 0;
                for(int k = 0; k < sz[i + 1]; k++){
                    wsm[i][j][k] += mr[i][j] * mods[i + 1][k] / (derRelu_l(mr[i][j]) * 256);
                    mods[i][j] += weights[i][j][k] * mods[i + 1][k] / (derRelu_l(mr[i][j]) * 256);
                    bsm[i][j] += mods[i + 1][k] / derRelu_l(mr[i][j]);
                }
            }
        }
    }
    void backpropagation_save(int cases){
        for(int i = 0; i < n - 1; i++){
            for(int j = 0; j < sz[i]; j++){
                biases[i][j] += bsm[i][j] / cases;
                bsm[i][j] = 0;
                for(int k = 0; k < sz[i + 1]; k++)
                    if(wsm[i][j][k] / cases != 0)
                        weights[i][j][k] += wsm[i][j][k] / cases,//   cout << "ws[" << i << "][" << j << "][" << k << "] += " << wsm[i][j][k] << '\n',
                        wsm[i][j][k] = 0;
            }
        }
        for(int i = 0; i < sz[n - 1]; i++)
            biases[n - 1][i] = bsm[n - 1][i] / cases, //cout << "bs[" << n - 1 << "][" << i << "] += " << bsm[n - 1][i] / cases << '\n',
            bsm[n - 1][i] = 0;
    }
    int mistake(int* real_ans){
        int ans = 0;
        for(int i = 0; i < sz[n - 1]; i++){
            ans += pow(mr[n - 1][i] - real_ans[i], 2) / 256;
        }
        return ans;
    }
};

int main(){
    pI* ps = readPict();
    int n = 4, sz[] = {784, 16, 16, 10};
    ai ir(sz, n);
    int cases = 10, mid_mistake = 0, cur, correct_anses;
    for(int j = 0; j < 60; j++)
    for(int i = 0; i < 600 * cases; i += cases){
        mid_mistake = 0;
        correct_anses = 0;
        for(int k = 0; k < cases; k++){
            int *a = ir.calc_ans(ps[i + k].pix);
            /*
            for(int j = 0; j < sz[n - 1]; j++)
                cout << j << ") " << a[j] << '\t';
            cout << "ai ans: " << max_el(a, sz[n - 1]) << ", real ans: " << (int) ps[i + k].ans << '\n'; // */
            ir.backpropogation_set( generate_ans(ps[i + k].ans, sz[n - 1]) );
            //cout << "mistake: ";
            cur = ir.mistake( generate_ans(ps[i + k].ans, sz[n - 1]) );
            if(max_el(a, sz[n - 1]) == ps[i + k].ans)
                correct_anses++;
            mid_mistake += cur;
            cout << cur << '\n' << '\n';
        }
        cout << "middle mistake: " << mid_mistake << ", ai precision: " << correct_anses << " / " << cases << '\n';
        ir.backpropagation_save(cases * 60);
    }
}