#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

int* generateAns(int ans, short len){
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

int leakyDiv = 3;

int relu_leaky(int n){
    if(n>=0) return n;
    return n/leakyDiv;
}

char derRelu_leaky(int n){
    if(n>=0) return 1;
    else return leakyDiv;
}

//// Prossec image
struct pI{
    char ans;
    unsigned char rs, cs;
    char pix[784];
};

class ai
{
private:
    int ***weights, ***wsMod, **biases, **bsMod, **midReses;
public:
    short *layers, layN;
    void setup(short *lays, short n){
        layers = lays;
        layN = n;
        weights = new int**[layN-1];
        wsMod = new int**[layN-1];
        biases = new int*[layN-1];
        bsMod = new int*[layN-1];
        for(short i=0; i<layN-1; i++){
            biases[i] = new int[layers[i+1]];
            bsMod[i] = new int[layers[i+1]];
            weights[i] = new int*[layers[i+1]];
            wsMod[i] = new int*[layers[i+1]];
            if(i!=layN-1)
            for(short j=0; j<layers[i]; j++){
                biases[i][j] = rand()%255 - 128;
                weights[i][j] = new int[layers[i]];
                wsMod[i][j] = new int[layers[i]];
                for(short k=0; k<layers[i]; k++)
                    weights[i][j][k] = rand()%255 - 128;
            }
        }
        midReses = new int*[layN];
        for(short i=0; i<layN; i++)
            midReses[i] = new int[layers[i]];
    }
    int **reses(char *inp){
        for(short i=0; i<layers[0]; i++)
            midReses[0][i] = (int)((inp[i]+256)%256);
        
        for(short i=0; i<layN-1; i++){
            for(short j=0; j<layers[i+1]; j++){
                midReses[i+1][j] = biases[i][j];
                for(short k=0; k<layers[i]; k++){
                    midReses[i+1][j] += relu_leaky(midReses[i][j]*weights[i][j][k]/256);
        }}}

        return midReses;
    }
    void backpropagation_count(int **midReses, int *anses){
        int **add = new int*[layN];
        for(int i=0; i<layN; i++)
            add[i] = new int[layers[i]];
        for(int i=0; i<layers[layN-1]; i++)
            add[layN-1][i] += 2*(midReses[layN-1][i] - anses[i]),
            bsMod[layN-1][i] += add[layN-1][i] / derRelu_leaky(midReses[layN-1][i]);

        for(int i=layN-2; i>=0; i--){
            for(int j=0; j<layers[i]; j++){
                bsMod[i][j] += add[i+1][j] / derRelu_leaky(midReses[i][j]);
                for(int k=0; k<layers[i+1]; k++){
                    wsMod[i][j][k] += add[i+1][j] * midReses[i][j] / derRelu_leaky(midReses[i][j]);
                    add[i][k] += add[i+1][j] * weights[i][j][k] / derRelu_leaky(midReses[i][j]);
                }
            }
        }
    }
    void backpropagation_res(int count){
        //cout<<"ai::backpropagation_result\n";
        for(int i=0; i<layN-1; i++)
            for(int j=0; j<layers[i]; j++){
                if(i!=0)
                    cout<<bsMod[i][j]<<'\n';
                biases[i][j] += bsMod[i][j]/count;
                //
                //cout<<"biases["<<i<<"]["<<j<<"] += "<<min(5,max(-5,bsMod[i][j]/count))<<'\n';//<<" ("<<lastToCost[i][j]<<" * "<<derRelu(midReses[i][j])<<")"<<'\n';
                bsMod[i][j] = 0;
                for(int k=0; k<layers[i+1]; k++)
                    weights[i][j][k] += wsMod[i][j][k]/count,
                    wsMod[i][j][k] = 0;
            }
    }
    ll mistakes(int* inp, int* anses, short len){
        ll ans;
        for(short i=0; i<len; i++)
            ans+=(int)pow(inp[i]-anses[i], 2) / 255;
        return ans;
    }
};

void trainAI(ai *ml, pI *pics, int len){
    short lays[] = {784, 16, 16, 10}, trSet=10;
    ml->setup(lays, 4);
    int **reses;
    ll mistakes;
    for(int i=0; i<4*trSet; i+=trSet){
        mistakes=0;
        for(int j=0; j<trSet; j++){
            reses = ml->reses(pics[i+j].pix);
            mistakes += ml->mistakes(reses[ml->layN-1], generateAns(pics[i+j].ans, ml->layers[ml->layN-1]), ml->layers[ml->layN-1]);
            ml->backpropagation_count(reses, generateAns(pics[i+j].ans, ml->layers[ml->layN-1]));
        }
        ml->backpropagation_res(trSet);
        cout<<"mid mistake: "<<mistakes/trSet<<"\n";
    }
}

int pLen;

pI* readPict(){
    fstream ansI, pixI;
    ansI.open("siteOut/cpp/train-labels.gz");
    pixI.open("siteOut/cpp/train-images.gz");
    unsigned char* vals=new unsigned char[4];
    uint32_t headers[4]; // Magic NUM | N (ROWs | COLs)
    for(int i=0; i<2; i++)
        for(int j=0; j<4; j++)
            ansI>>vals[j],
            headers[i]=headers[i]+(vals[j]<<(8*(3-j)));
    
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            pixI>>vals[j],
            headers[i]=(headers[i]<<8)+vals[j];
    cout<<"magic N: "<<headers[0]<<'\n'
        <<"N: "<<headers[1]<<'\n'
        <<"rs: "<<headers[2]<<'\n'
        <<"cs: "<<headers[3];
    delete[] vals;
    cout<<'\n';
    pI* photos = new pI[headers[1]];
    for(int i=0; i<headers[1]; i++){
        ansI.read(&(photos[i].ans),1);
        //cout<<(unsigned int)photos[i].ans<<'\n';
        photos[i].rs=headers[2];
        photos[i].cs=headers[3];
    }
    for(int j=0; j<headers[1]; j++){
        for(int row=0; row<headers[2]; row++)
            for(int col=0; col<headers[3]; col++)
                pixI.read(&photos[j].pix[row*photos[j].rs+col],1);
    }
    pLen=headers[1];
    return photos;
}

int main(){
    pI *photos = readPict();
    ai mAI;
    trainAI(&mAI, photos, pLen);
    return 0;
}