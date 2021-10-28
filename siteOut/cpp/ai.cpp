#include <iostream>
#include <cmath>
#include <ostream>
#include <fstream>
#include <iomanip>

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
    ansI.open("cpp/train-labels");
    pixI.open("cpp/train-images");
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
    /*cout << "magic N: " << headers[0] << '\n'
        << "N: "  << headers[1] << '\n'
        << "rs: " << headers[2] << '\n'
        << "cs: " << headers[3] << '\n';*/
    delete[] vals;
    pI* photos = new pI[int(headers[1] + headers[1] * 0.1)];
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
    pLen = headers[1];// + headers[1] * 0.1;

    /*randomizing some pics
    for(int i = headers[1] * 0.1; i >= 0; i--){
        int k = headers[1] + i;
        for(int j = 0; j <= 784; j++)
            photos[k].pix[j] = rand() % 256 - 128;
        photos[k].ans  = 10;
    }//*/

    random_shuffle(photos, photos + pLen);
    return photos;
}

class ai{
    int ***weights, **biases, *sz, n, **mr, ***wsm, **bsm, **mods;

    public:
    //initialization of ws and bs
    ai(int intstr[], int sz_len, bool writen, int maxInp){
        fstream save;
        save.open("ai_data/ai");
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
                if(writen)
                    save.read((char*) (biases[i] + j), sizeof(biases[i][j]));
                else biases[i][j] = rand() % 256 - 128;
                weights[i][j] = new int[sz[i + 1]];
                wsm[i][j] = new int[sz[i + 1]];
                for(int k = 0; k < sz[i + 1]; k++){
                    if(writen)
                        save.read((char*) (weights[i][j] + k), sizeof(weights[i][j][k]));
                    else weights[i][j][k] = rand() % 256 - 128;
                }
            }
        }
        biases[n - 1] = new int[sz[n - 1]];
        bsm[n - 1] = new int[sz[n - 1]];
        mr[n - 1] = new int[sz[n - 1]];
        mods[n - 1] = new int[sz[n - 1]];
        for(int i = 0; i < sz[n - 1]; i++){
            if(writen)
                save.read((char*) (biases[n - 1] + i), sizeof(biases[n - 1][i]));
            else biases[n - 1][i] = rand() % 256 - 128;
        }
        save.close();
    }

    void save_ai(){
        // saving ai indeses
        ofstream save;
        save.open("ai_data/ai", ios::binary);
        save.clear();
        for(int i = 0; i < n - 1; i++){
            for(int j = 0; j < sz[i]; j++){
                save.write((char*) (biases[i] + j), sizeof(biases[i][j]));
                for(int k = 0; k < sz[i + 1]; k++)
                    save.write((char*) (weights[i][j] + k), sizeof(weights[i][j][k]));
            }
        }
        for(int i = 0; i < sz[n - 1]; i++)
            save.write((char*) (biases[n - 1] + i), sizeof(biases[n - 1][i]));
        save.close();
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
            bsm[n - 1][i] += mods[n - 1][i];
            //cout << i << ") " << mods[n - 1][i] << '\t';
        //cout << "last modify\n";
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
    cout << "pLen: " << pLen << '\n';
    int n = 4, sz[] = {784, 16, 16, 10};
    ai ir(sz, n, true, 255);
    int cases = 10, mid_mistake = 0, cur, correct_anses = 0, part_cor_ans = 0, cur_cor, tstMore = 20;
    int *wrongs = new int [sz[n - 1]];
    for(int j = 0; j < tstMore; j++){
    for(int i = 0; i < pLen; i += cases){
        mid_mistake = 0;
        cur_cor = 0;
        for(int k = 0; k < cases; k++){
            int *a = ir.calc_ans(ps[i + k].pix);
            ir.backpropogation_set( generate_ans(ps[i + k].ans, sz[n - 1]) );
            cur = ir.mistake( generate_ans(ps[i + k].ans, sz[n - 1]) );
            if(max_el(a, sz[n - 1]) == ps[i + k].ans)
                cur_cor++;
            else
                wrongs[ps[i + k].ans]++,
                wrongs[max_el(a, sz[n - 1])]++;
            mid_mistake += cur;
            //cout << cur << '\n' << '\n';
        }
        //cout << i + 1 << ")\tmiddle mistake: " << mid_mistake << ",\t ai precision: " << cur_cor << " / " << cases << '\n';
        ir.backpropagation_save(cases * 65);
        part_cor_ans += cur_cor;
    }
        cout << j + 1 << ")  correct anses: " << part_cor_ans << " / " << pLen << "\t so: " << fixed << setprecision(1) << (double) part_cor_ans * 100 / pLen << "%\n"; 
        cout << "wrong for i(0..11): ";
        for(int i = 0; i < sz[n - 1]; i++)
            cout << i << ") " << wrongs[i] << ", ",
            wrongs[i] = 0;
        cout << '\n';
        correct_anses += part_cor_ans;
        part_cor_ans = 0;
        ir.save_ai();
    }
    cout << "correct results: " << correct_anses << " / " << pLen * tstMore << " is " << setprecision(1) << fixed << (double)correct_anses * 100 / (pLen*tstMore) << "%" << '\n';
    //*/
}


/*

s

*/