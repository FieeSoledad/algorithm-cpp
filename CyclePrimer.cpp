//
// Created by hsl on 2024/6/23.
//

//洛谷P1217 回文质数
//无语啦，这个都能超时


/*
 * 首先找质数，然后找回文质数
 * 找质数，使用质数筛
 * 欧拉筛和埃氏筛
 * 埃氏筛时间复杂度O(n*log(logn))
 * 欧拉筛是线性的时间复杂度
 * 错误：判断回文我tm都不过脑子的吗？
 * 为什么边界是根号n，因为大于根号n的肯定已经判断过了
 */

#include<bits/stdc++.h>
using namespace std;

int n = 100;
bool isPrimer[101];

/**
 * 100以内有25个质数
 */
void count(){
    for(int idx=2;idx*idx<=100;idx++){
        if(!isPrimer[idx]){
            for(int j=idx*idx;j<=100;j+=idx){
                isPrimer[j]=1;
            }
        }
    }
    int count = 0;
    for(int i=2;i<=n;i++){
        if(!isPrimer[i]){
            count++;
        }
    }

    cout<<count<<endl;
}

/**
 * 这种方法虽然能解题，但是还是会超时，有点难受
 * @param a
 * @return
 */
int isHuiwen(int a){
    int tmp = a;
    int reverse = 0;
    while(a!=0){
        reverse=a%10+reverse*10;
        a = a/10;
    }
    //cout<<tmp<< ":"<<reverse<<endl;
    if(reverse==tmp){
        return 1;
    }else{
        return 0;
    }
}

/**
 * 在c语言中，数组传递会退化为指针
 */
void print(bool v[]){
    for(auto& item:v){
        cout<<item<<" ";
    }
}

/**
 *
 * @return
 */
int main(){
    int a,b;
    cin>>a>>b;
    bool visited[b+1];
    for(int idx=0;idx<=b;idx++){
        visited[idx] = 0;
    }

    for(int i=2;i*i<=b;i++){
        if(!visited[i]){
            for(int j=i*i;j<=b;j+=i){
                visited[j]=1;
            }
        }
    }
    for(auto& item:visited){
        cout<< item<<" ";
    }
    for(int idx=a;idx<=b;idx++){
        if(!visited[idx] && isHuiwen(idx)){
            cout<<idx<<endl;
        }
    }
    return 0;
}
