//
// Created by hsl on 2024/11/15.
//

// 除法同余和逆元
// 除法同余的使用场景，a/b%mod 三个条件：1a|b 2mod is primer 3gcd(b,mod)=1
// b的逆元是 b**(mod-2)%mod
// 除法同余的使用 a/b%mod = (a%mod)*(b的逆元)%mod
//#include<iostream>

//这题可见cstdio的输入输出效率高于iostream？
#include<cstdio>
using namespace std;
//连续数字逆元的线性递推
const int MAX = 3*1000000+1;
int inv[MAX];
int main(){
    int n,p;
    scanf("%d%d",&n,&p);
    inv[1] = 1;
    printf("1\n");
    for(int i=2;i<=n;i++){
        //这样才能避免溢出吧
        inv[i] = p-(long)(p/i)*inv[p%i]%p;
        printf("%d\n",inv[i]);
    }
    return 0;
}