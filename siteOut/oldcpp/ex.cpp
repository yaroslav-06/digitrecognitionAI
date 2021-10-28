#include <iostream>

using namespace std;

int main(){
    int i=2000, ans=0;
    for(;i<=2010;i++)
        ans+=i/1000 + (i/100)%10+i%10;
    cout<<ans;
}

