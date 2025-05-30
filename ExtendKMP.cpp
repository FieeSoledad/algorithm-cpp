//
// Created by hsl on 2025/1/20.
//
#include<iostream>
#include<cstring>
using namespace std;
typedef long long ll;

const int MAXN = 2*1e7+1;
char a[MAXN];
char b[MAXN];
int bz[MAXN];
int e[MAXN];
int alen;
int blen;

/**
 * 求Z数组
 * @param s
 */
void zarray(char* s,int* z,int slen){
    z[0] = slen;
    for(int i=1,c=1,r=1,len;i<slen;i++){
         len = r>i ? min(z[i - c], r - i) : 0;
         while(i+len<slen && s[i+len]==s[len]){
             len++;
         }
         if(i+len>r){
             r = i+len;
             c = i;
         }
        z[i] = len;
    }
}

/**
 * 求e数组，即a串和b串的LCP(Longest Common Prefix )
 * @param a
 * @param b
 * @param bz
 * @param e
 */
void earray(char* a,char* b,int* bz,int* e){
    zarray(b, bz, blen);
    for(int i=0,c=0,r=0,len;i<alen;i++){
        len = r>i?min(bz[i-c],r-i):0;
        while(i+len<alen && len<blen && a[i+len]==b[len]){
            len++;
        }
        if(i+len>r){
            r = i+len;
            //TODO ERROR
            c = i;
        }
        e[i] = len;
    }
}

ll eor(int* arr,int n){
    ll ans = 0;
    for(int idx=0;idx<n;idx++){
        ans^=((ll)idx+1)*(arr[idx]+1);
    }
    return ans;
}

//Z函数
//manacher算法思想
//AZ[index]：从index出发和字符串本身最长公共前缀
//https://www.luogu.com.cn/problem/P5410
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    scanf("%s",a);
    scanf("%s",b);
    string s;
//    zarray(b,bz);
    alen = strlen(a);
    blen = strlen(b);
    earray(a, b, bz, e);
    ll ans1 = eor(bz, blen);
    ll ans2 = eor(e,alen);
    cout<<ans1<<endl;
    cout<<ans2<<endl;
    return 0;
}
