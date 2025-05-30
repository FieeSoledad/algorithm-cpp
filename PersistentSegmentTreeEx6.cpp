//
// Created by hsl on 2025/4/7.
//
#include<iostream>
using namespace std;

const int MAXN = 2*1e5+1;
const int MAXT = MAXN*22;//这个为什么是22倍？

int m,n;//操作数和数组大小
int arr[MAXN];
int root[MAXN];//多个版本的线段树的头节点
int ln[MAXT];//左孩子
int rn[MAXT];//右孩子
int lateLeft[MAXT];//维护范围中每个数字出现的最晚位置的最左位置在哪
int cnt;//空间计数器

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

/**
 * 假设查询300-900
 * @param pos 300
 * @param l
 * @param r
 * @param i 900
 * @return
 */
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
    cnt = 0;
    root[0] = build(0,n);
    for(int idx=1;idx<=n;idx++){
        if(arr[idx]>n){//不需要维护，因为不可能是最晚出现的位置
            root[idx] = root[idx-1];
        }else{
            root[idx] = update(arr[idx],idx,0,n,root[idx-1]);
        }
    }
}


//让我们了解值域线段树和权值线段树的区别是什么
//https://www.luogu.com.cn/problem/P4137
//区间内没有出现的最小自然数
//定义了一个很绕的区间信息，一个区间中的数字最晚出现的下标
//前提是0也是自然数
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int idx=1;idx<=n;idx++){
        cin>>arr[idx];
    }
    prepare();
    for(int idx=1;idx<=m;idx++){
        int l,r;
        cin>>l>>r;
        cout<<query(l,0,n,root[r])<<endl;
    }
    return 0;
}