#include <iostream>
#include <fstream>
#include "pI.h"

using namespace std;

template <class T>
void random_shuffle(T m[], int len){
    for(int i = 0; i < len; ++i)
        swap(m[i], m[rand()%len]);
}

pics readPict(bool train){
    fstream ansI, pixI;
    if(train){
        cout << "training...\n";
        ansI.open("../data/train-labels");
        pixI.open("../data/train-images");
    }else{
        cout << "testing...\n";
        ansI.open("../data/test-labels");
        pixI.open("../data/test-images");
    }
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
    /*
    cout << "magic N: " << headers[0] << '\n'
        << "N: "  << headers[1] << '\n'
        << "rs: " << headers[2] << '\n'
        << "cs: " << headers[3] << '\n';//*/
    delete[] vals;
    pic* photos = new pic[int(headers[1] + headers[1] * 0.1)];
    for(int i = 0; i < headers[1]; i++){
        ansI.read(&(photos[i].ans), 1);
        photos[i].rs = headers[2];
        photos[i].cs = headers[3];
    }


    for(int j = 0; j < headers[1]; j++){
        for(int row = 0; row < headers[2]; row++)
            for(int col = 0; col < headers[3]; col++)
                pixI.read(&photos[j].pix[row * photos[j].rs + col], 1);
    }

    random_shuffle(photos, headers[1]);
    return {photos, (int)headers[1]};
}
