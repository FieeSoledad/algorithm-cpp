//
// Created by hsl on 2024/12/7.
//

#include<iostream>
#include<climits>
#include<random>
using namespace std;

const int MAXN = 100000+1;
int cnt=0;
int head=0;
int key[MAXN];
int lnode[MAXN];
int rnode[MAXN];
int stsize[MAXN];
int n,m;
//TODO 这里数据类型设置错误，导致不够平衡诶
double priority[MAXN];
//维护翻转的懒信息
bool rev[MAXN];
//用于收集中序遍历的答案
int ans[MAXN];
int ansci = 0;

void up(int i){
    stsize[i] = stsize[lnode[i]]+stsize[rnode[i]]+1;
}

//懒信息下发
void down(int i){
    if(rev[i]){
        int temp = lnode[i];
        lnode[i] = rnode[i];
        rnode[i] = temp;
        rev[lnode[i]] = !rev[lnode[i]];
        rev[rnode[i]] = !rev[rnode[i]];
        rev[i] = false;
    }
}

//根据排名分裂
//排名小于等于rk的在左，大于等于的在右
void split(int l,int r,int cur,int rk){
    if(cur==0){
        rnode[l]=lnode[r] = 0;
    }else{
        down(cur);
        if(stsize[lnode[cur]]+1<=rk){
            rnode[l] = cur;
            //TODO cur指针移动错误 lnode下标错误
            split(cur,r,rnode[cur],rk-stsize[lnode[cur]]-1);
        }else{
            lnode[r] = cur;
            split(l,cur,lnode[cur],rk);
        }
        up(cur);
    }
}

int merge(int l,int r){
    if(l==0 || r==0){
        return l+r;
    }
    if(priority[l]>=priority[r]){
        down(l);
        rnode[l] = merge(rnode[l],r);
        up(l);
        return l;
    }else{
        down(r);
        lnode[r] = merge(l,lnode[r]);
        up(r);
        return r;
    }
}

void mid_order_travel(int i){
    if(i!=0){
        down(i);
        mid_order_travel(lnode[i]);
        ans[++ansci]=key[i];
        mid_order_travel(rnode[i]);
    }
}

//https://www.luogu.com.cn/problem/P3391
//翻转一个区间
//文艺平衡树，用于维护有序结构，并支持返回翻转
//类似线段树的懒信息存储
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int idx=1;idx<=n;idx++){
        key[++cnt] = idx;
        stsize[cnt] = 1;
        priority[cnt] = (double)rand()/RAND_MAX;
        head = merge(head,cnt);
    }

    for(int idx=1;idx<=m;idx++){
        int op1,op2;
        cin>>op1>>op2;
        split(0,0,head,op2);
        int r1 =  lnode[0];
        int l1 = rnode[0];
        //TODO 这里op1-1
        split(0,0,l1,op1-1);
        int r2 = lnode[0];
        int l2 = rnode[0];
        rev[r2] = !rev[r2];
        head = merge(merge(l2,r2),r1);
    }
    mid_order_travel(head);
    for(int idx=1;idx<=n;idx++){
        cout<<ans[idx]<<" ";
    }
    return 0;
}
