//
// Created by hsl on 2025/2/5.
//
#include<iostream>
#include<string>
typedef long long ll;
using namespace std;

const int BASE = 433;
const int MAXN = 1e4;
ll pow[MAXN];
ll hash_[MAXN];

void build(string s,int n){
    pow[0] = 1;
    for(int i=1;i<n;i++){
        pow[i] = pow[i-1]*BASE;
    }
    hash_[0] = s[0]-'a'+1;
    for(int i=1;i<n;i++){
        hash_[i] = hash_[i-1]*BASE+s[i]-'a'+1;
    }
}


//确实得卡控l=0的情况
ll get_hash_code(int l,int r){
    ll ans = hash_[r];
    if(l>0){
        ans-=hash_[l-1]*(r-l+1);
    }
    return ans;
}

int strStr(string haystack, string needle) {
    build(haystack,haystack.size());
    ll target = needle[0]-'a'+1;
    for(int i=1;i<needle.size();i++){
        //TODO 这里是+不是+=
        target = target*BASE+needle[i]-'a'+1;
    }
    int len = needle.size();
    for(int i=0;i+len<=haystack.size();i++){
        ll cur = get_hash_code(i,i+len-1);
        if(cur==target){
            return i;
        }
    }
    return -1;
}


//实现以O1的时间复杂度获取任意子串的哈希值
//https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/
//可以用于代替KMP算法
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s1,s2;
    cin>>s1>>s2;
    cout<<strStr(s1,s2)<<endl;
    return 0;
}