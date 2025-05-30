//
// Created by hsl on 2025/4/7.
//
#include<iostream>
using namespace std;

const int MAXN = 2*1e5+1;
const int MAXT = MAXN*22;//这个为什么是22倍？

int m,n;
int arr[MAXN];
int root[MAXN];
int ln[MAXT];
int rn[MAXT];
int lateLeft[MAXT];//维护范围中每个数字出现的最晚位置的最左位置在哪
int cnt;

int build(int l,int r){
    int id=++cnt;
    lateLeft[id]=0;
    if(l<r){
        int mid = (l+r)>>1;
        ln[id] = build(l,mid);
        rn[id] = build(mid+1,r);
    }
    return id;
}


/**
 * 值域线段树的更新
 * 这是将jobi这个数字最晚出现的下标改为jobv
 * @param jobi
 * @param jobv
 * @param l
 * @param r
 * @param mid
 * @return
 */
int update(int jobi,int jobv,int l,int r,int i){
    int id = ++cnt;
    ln[id] = ln[i];
    rn[id] = rn[i];
    if(l==r){
        lateLeft[id] = jobv;
    }else{
        int mid = (l+r)>>1;
        if(jobi<=mid){
            ln[id] = update(jobi,jobv,l,mid,ln[id]);
        }
        if(jobi>mid){
            rn[id] = update(jobi,jobv,mid+1,r,rn[id]);
        }
        lateLeft[id] = min(lateLeft[ln[id]],lateLeft[rn[id]]);
    }
    return id;
}

int query(int pos,int l,int r,int i){
    if(l==r){
        return l;
    }
    int mid = (l+r)>>1;
    if(lateLeft[ln[i]]<pos){//说明缺的在左侧
        return query(pos,l,mid,ln[i]);
    }else{
        return query(pos,mid+1,r,rn[i]);
    }
}


/**
 * 构建初始的可持久化线段树
 */
void prepare(){

}
//让我们了解值域线段树和权值线段树的区别是什么
//https://www.luogu.com.cn/problem/P4137
//区间内没有出现的最小自然数
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout<<"hello world"<<endl;
    return 0;
}