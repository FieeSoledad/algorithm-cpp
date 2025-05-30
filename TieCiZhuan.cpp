//
// Created by hsl on 2024/11/21.
//

//http://poj.org/submit?problem_id=2411
//贴瓷砖问题

#include<iostream>
#include<vector>
using namespace std;
typedef long long ll;

int h,w;

int getBitOfIndex(int num,int index){
    return (num>>index)&1;
}

void setOne(int& num,int index){
    num = num | (1<<index);
    return;
}

void setZero(int& num,int index){
    num = num & (~(1<<index));
    return;
}

ll func(int i,int j,int state,vector<vector<vector<ll> > >& memory){
    if(i==h){
        return 1;
    }
    if(j==w){
        return func(i+1,0,state,memory);
    }
    if(memory[i][j][state]!=-1){
        return memory[i][j][state];
    }
    //尝试策略需要什么信息呢
    ll ans = 0;
    //上一行竖砖不影响我
    if(getBitOfIndex(state,j)==0){
        //可以贴横砖、可以贴竖砖
        //还需要避免越界诶
        if(i<h-1){
            setOne(state,j);
            ans+= func(i,j+1,state,memory);
            setZero(state,j);
        }
        //能否贴横砖呢?
        if(j<w-1 && getBitOfIndex(state,j+1)==0){
            ans+= func(i,j+2,state,memory);
        }
    }else{
        setZero(state,j);
        ans += func(i,j+1,state,memory);
    }
    memory[i][j][state] = ans;
    return ans;
}

//这么看cpp中的引用就是Rust中的类似于可变引用
//我总是遇见没办法正常输入退出的情况，经常是因为少了一个\n
int main(){
    //11 11的时候出状况了，woc
    while(cin>>h>>w){
        if(h==0 && w==0){
            break;
        }
        vector<vector<vector<ll> > > memory(h,vector<vector<ll> >(w,vector<ll>(1<<11,-1)));
        //被我自己蠢哭
        cout<<func(0,0,0,memory)<<endl;
    }
    return 0;
}
