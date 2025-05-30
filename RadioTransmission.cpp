//
// Created by hsl on 2024/12/11.
//

#include<iostream>
#include<string>
#include<vector>
using namespace std;
//求next数组
vector<int> next_arr(string s){
    int len = s.size();
    if(len==1){
        vector<int> v(1,0);
        return v;
    }
    vector<int> ans(len+1,0);
    ans[0] = -1;
    ans[1] = 0;
    int index = 2;
    int cn = 0;
    while(index<=len){
        if(s[index-1]==s[cn]){
            ans[index++] =++cn;
        }else if(cn>0){
            cn = ans[cn];
        }else{
            ans[index++] = 0;
        }
    }
    return ans;
}

//https://www.luogu.com.cn/problem/P4391
//kmp算法最短循环节的的应用
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    string s;
    cin>>n;
    cin>>s;
    vector<int> nextArr = next_arr(s);
    cout<<n-nextArr[n]<<endl;
    return 0;
}
