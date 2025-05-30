//
// Created by hsl on 2025/4/5.
//
#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 1e5+1;
const int MAXT = MAXN*70;//最坏情况，全都C操作  2m*logn+2n*logn 需要复制左右节点吧
int n,m,t = 0;
int arr[MAXN];
int root[MAXN];
int ln[MAXT];
int rn[MAXT];
ll sum[MAXT];
ll add[MAXT];
int cnt = 0;//空间计数

int clone(int i){
    int id = ++cnt;
    ln[id] = ln[i];
    rn[id] = rn[i];
    sum[id] = sum[i];
    add[id] = add[i];
    return id;
}

void up(int i){
    sum[i] = sum[ln[i]]+sum[rn[i]];
}

void lazy(int i,ll value,int count){
    sum[i] += value*count;
    add[i] += value;
}

void down(int i,int lcnt,int rcnt){
    if(add[i]!=0){
        ln[i] = clone(ln[i]);//这里之所以直接新建节点，因为不会在一个版本上反复操作
        rn[i] = clone(rn[i]);
        lazy(ln[i],add[i],lcnt);
        lazy(rn[i],add[i],rcnt);
        add[i] = 0;
    }
}

int build(int l,int r){
    int id = ++cnt;
    if(l==r){
        //TODO 这里之前的错误
        sum[id] = arr[l];
    }else{
        int mid = (l+r)>>1;
        ln[id] = build(l,mid);
        rn[id] = build(mid+1,r);
        up(id);
    }
    add[id] = 0;
    return id;
}

int range_add(int jobl,int jobr,ll value,int l,int r,int i){//执行范围增强的任务
    i = clone(i);
    //先看能不能懒住
    if(jobl<=l && jobr>=r){
        lazy(i,value,r-l+1);
    }else{
        int mid = (l+r)>>1;
        down(i,mid-l+1,r-mid);
        if(jobl<=mid){
            ln[i] = range_add(jobl,jobr,value,l,mid,ln[i]);
        }
        if(jobr>mid){
            rn[i] = range_add(jobl,jobr,value,mid+1,r,rn[i]);
        }
        up(i);
    }
    return i;
}

ll range_query(int jobl,int jobr,int l,int r,int i){//执行范围查询的任务
    if(jobl<=l && jobr>=r){
        return sum[i];
    }else{
        int mid = (l+r)>>1;
        down(i,mid-l+1,r-mid);//这里有重连
        ll ans = 0;
        if(jobl<=mid){
            ans+=range_query(jobl,jobr,l,mid,ln[i]);
        }
        if(jobr>mid){
            ans+=range_query(jobl,jobr,mid+1,r,rn[i]);
        }
        return ans;
    }

}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    root[0] = build(1,n);
    for(int i=1;i<=m;i++){
        char op = 0;
        int l,r,d,time;
        cin>>op;
        if(op=='C'){//新建
            cin>>l>>r>>d;
            root[t+1] = range_add(l,r,d,1,n,root[t]);
            t+=1;
        }else if(op=='Q'){
            cin>>l>>r;
            cout<<range_query(l,r,1,n,root[t])<<endl;
        }else if(op=='H'){
            cin>>l>>r>>time;
            cout<<range_query(l,r,1,n,root[time])<<endl;
        }else if(op=='B'){
            cin>>time;
            t = time;
        }
    }
    return 0;
}
