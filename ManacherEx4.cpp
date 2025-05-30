//
// Created by hsl on 2025/1/21.
//
#include<iostream>
#include<string>
using namespace std;

const int MAXN = 2*1e5+1;
char ss[MAXN<<1];
int p[MAXN<<1];
int length;
int ans = 2;
int left_len[MAXN<<1];
int right_len[MAXN<<1];

//获取处理串
void manacherss(string s,char* ss){
    length = 2*s.size()+1;
    for(int i=0,j=0;i<length;i++){
        ss[i] = (i&1)==0?'#':s[j++];
    }
}

void manacher(string s){
    manacherss(s,ss);
    for(int i=0,c=0,r=0,len;i<length;i++){
        len = r>i?min(r-i,p[2*c-i]):1;
        while(i+len<length && i-len>=0 && ss[i+len]==ss[i-len]){
            len++;
        }
        if(i+len>r){
            r = i+len;
            c = i;
        }
        p[i] = len;
        //获取结果
    }

    //最早产生影响的就是最大的
    for(int i=0,j=0;i<length;i++){
        while(i+p[i]>j){
            left_len[j] = j-i;
            j+=2;
        }
    }

    for(int i = length-1,j=length-1;i>=0;i--){
        while(i-p[i]<j){
            right_len[j] = i-j;
            j-=2;
        }
    }
    for(int i=2;i<length-3;i+=2){
        ans = max(ans,left_len[i]+right_len[i]);
    }
    return;
}

//https://www.luogu.com.cn/problem/P4555
//双回文子串的长度
//这题的最大化保障有点意思
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin>>s;
    manacher(s);
    cout<<ans<<endl;
    return 0;
}

