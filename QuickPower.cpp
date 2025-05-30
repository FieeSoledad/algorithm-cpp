//
// Created by hsl on 2024/12/7.
//
#include<iostream>
using namespace std;
typedef long long ll;



int power(ll a,int b,int p){
    ll ans = 1;
    while(b>0){
        if((b&1)==1){
            ans = ans*a % p;
        }
        //TODO 这里a可能会爆int
        a = a*a %p;
        b>>=1;
    }
    return ans;
}

//快速幂/龟速乘模板
//https://www.luogu.com.cn/problem/P1226
//其实就是在乘法过程中取模防止溢出
int main(){
    int a,b,p;
    cin>>a>>b>>p;
    cout<<a<<"^"<<b<<" mod "<<p<<"="<<power(a,b,p)<<endl;
    return 0;
}
