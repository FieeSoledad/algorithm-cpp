//
// Created by hsl on 2024/12/12.
//
#include<iostream>
#include<cstring>
using namespace std;


const int MAXN = 1e7+1e6+1;
//原始串
char s[MAXN];
//目标串
char ss[MAXN<<2];
//回文半径数组
int p[MAXN<<2];
//扩展串的长度
int length;

//制造这个处理串
void manacherss(char* s,char* ss){
    length = 2*strlen(s)+1;
    for(int idx=0,j=0;idx<length;idx++){
        ss[idx] = ((idx&1) == 0 ) ? '#':s[j++];
    }
}

//返回最长回文子串最大值
int manacher(char* str){
    manacherss(str,ss);
    int ans = 1;
    for(int i=0,c=0,r=0,len;i<length;i++){
        //先给一个至少的回文半径
        len = r>i?min(p[2*c-i],r-i):1;
        while(i+len<length && i-len>=0 && ss[i+len]==ss[i-len]){
            len++;
        }
        if(i+len>r){
            r = i+len;
            c = i;
        }
        ans = max(ans,len);
        p[i] = len;
    }
    //回文半径和真实回文串长度的关系f
    return ans-1;
}


//完成所谓的manacher算法，解决
//扩展kmp算法是与manacher非常相关的
//寻找一个字符串的最长回文子串，使用简单的二维动态规划O(n**2)可以实现
//暴力方法寻找最长回文子串：使用星号扩展，不会错过奇数或者偶数的的情况，引入一个回文半径，这样时间复杂度也是On**2
//时间复杂度为O(n)
//https://www.luogu.com.cn/problem/P3805
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    scanf("%s",s);
    int ans = manacher(s);
    cout<<ans<<endl;
    return 0;
}