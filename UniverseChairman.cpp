//
// Created by hsl on 2024/6/23.
//
//洛谷P1781宇宙总统
#include<bits/stdc++.h>
using namespace std;

/*
 * 这里有个疑问，就是cpp的string究竟是什么
 * string本质是一个容器一个类
 */

//考察比较字符串罢了
int main(){
    int n;
    cin>>n;
    int id=0;
    string max = "0";

    string cur;
    for(int idx=0;idx<n;idx++){
        cin>>cur;
        int curSize = cur.size();
        int maxSize = max.size();
        if(curSize>maxSize||(curSize==maxSize&& cur>max)){
            max = cur;
            id=idx+1;
        }
    }
    cout<<id<<endl;
    cout<<max<<endl;
    return 0;
}
