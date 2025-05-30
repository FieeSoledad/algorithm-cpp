
//
// Created by hsl on 2024/12/11.
//
#include<iostream>
#include<unordered_set>
#include<string>
#include<algorithm>
using namespace std;
typedef long long ll;

const int base = 433;

const int MAXN = 10000+5;
int hashv[MAXN];

int v(char c){
    if(c>='0' && c<='9'){
        return c-'0'+1;
    }else if(c>='A' && c<='Z'){
        return c-'A'+11;
    }else if(c>='a' && c<='z'){
        return c-'a'+37;
    }
}

//ll自然溢出
ll hash_value(string s){
    ll ans = v(s[0]);
    for(int i=1;i<s.size();i++){
        ans =  ans*base+v(s[i]);
    }
    return ans;
}


//字符串哈希
//https://www.luogu.com.cn/problem/P3311
//自己实现字符串哈希
//int main(){
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//    int n;
//    cin>>n;
//    unordered_set<string> s;
//    for(int idx=0;idx<n;idx++){
//        string t;
//        cin>>t;
//        s.insert(t);
//    }
//    string t;
//    t[1];
//    cout<<s.size()<<endl;
//    return 0;
//}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin>>n;
    string s;
    for(int i=0;i<n;i++){
        cin>>s;
        hashv[i] = hash_value(s);
    }
    //排序后统计
    sort(hashv+0,hashv+n);
    int cnt = 1;
    for(int i=1;i<n;i++){
        if(hashv[i]!=hashv[i-1]){
            cnt++;
        }
    }
    cout<<cnt<<endl;
    return 0;
}
