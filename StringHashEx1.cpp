//
// Created by hsl on 2025/1/22.
//

#include<iostream>
#include<string>
#include<cstring>
#include<unordered_set>
typedef long long ll;
using namespace std;


const int BASE = 433;
//由数字组成
int freq_table[10];
//集合，我使用字符串哈希有哈希碰撞的风险
unordered_set<ll> s;

//频率归0
void clear(){
    memset(freq_table,10,0);
}
//单个字符的映射
int v(char c){
    return c-'0'+1;
}

//https://leetcode.cn/problems/unique-substrings-with-equal-digit-frequency/
//独特子串，每个数字出现的频率都相同

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string cur;
    cin>>cur;
    cout<<cur<<endl;
    for(int i=0;i<cur.size();i++){
        clear();
        ll hash_code = 0;
        int maxCnt = 0,maxCntKinds = 0,kinds = 0;
        int curVal = 0;
        for(int j=i;j<cur.size();j++){
            curVal = cur[j]-'0';
            hash_code =  hash_code*BASE+v(cur[j]);
            freq_table[curVal]++;
            if(freq_table[curVal]==1){
                kinds++;
            }
            if(freq_table[curVal]==maxCnt){
                maxCntKinds++;
            }else if(freq_table[curVal]>maxCnt){
                maxCnt = freq_table[curVal];
                maxCntKinds = 1;
            }
            if(maxCntKinds == kinds){
                s.insert(hash_code);
            }
        }
    }
    cout<<s.size()<<endl;
    return 0;
}
