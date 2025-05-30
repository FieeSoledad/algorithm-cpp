//
// Created by hsl on 2025/4/6.
//
#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 1e5+1;
const int MAXT = MAXN*38;//为什么是25？
int n,m;
int arr[MAXN];
int root[MAXN];
int ln[MAXT];
int rn[MAXT];
ll sum[MAXT];
ll add_tag[MAXT];
int cnt = 0;
int t = 0;

int clone(int i){
    int id = ++cnt;
    ln[id] = ln[i];
    rn[id] = rn[i];
    sum[id] = sum[i];
    add_tag[id] = add_tag[i];
    return id;
}

void up(int i){
    sum[i] = sum[ln[i]]+sum[rn[i]];
}

int build(int l,int r){
    int id = ++cnt;
    if(l==r){
        sum[id] = arr[l];
    }else{
        int mid = (l+r)>>1;
        ln[id] = build(l,mid);
        rn[id] = build(mid+1,r);
        up(id);
    }
    add_tag[id] = 0;
    return id;
}

int range_add(int jobl,int jobr,int l,int r,ll value,int i){
    int id = clone(i);
    int a = max(jobl,l);
    int b = min(jobr,r);
    sum[id] += (b-a+1)*value;
    if(jobl<=l && jobr>=r){//懒住
        add_tag[id]+=value;
    }else{
        int mid = (l+r)>>1;
        if(jobl<=mid){
            ln[id] = range_add(jobl,jobr,l,mid,value,ln[id]);
        }
        if(jobr>mid){
            rn[id] = range_add(jobl,jobr,mid+1,r,value,rn[id]);
        }
    }
    return id;
}

ll range_query(int jobl,int jobr,int l,int r,ll his_tag,int i){
    if(jobl<=l && jobr>=r){
        int count = r-l+1;
        ll ans = sum[i]+count*his_tag;
        return ans;
    }else{
        int mid = (l+r)>>1;
        ll ans = 0;
        if(jobl<=mid){
            ans+= range_query(jobl,jobr,l,mid,his_tag+add_tag[i],ln[i]);
        }
        if(jobr>mid){
            ans+= range_query(jobl,jobr,mid+1,r,his_tag+add_tag[i],rn[i]);
        }
        return ans;
    }
}

//https://acm.hdu.edu.cn/showproblem.php?pid=4348
//To the moon
//使用标记永久化优化查询时的空间占用
//杭电卡空间这波
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    root[0] = build(1,n);
    for(int i=1;i<=m;i++){
        char op;
        int l,r,d,time;
        cin>>op;
        if(op=='C'){
            cin>>l>>r>>d;
            root[t+1] = range_add(l,r,1,n,d,root[t]);
            t+=1;
        }else if(op=='Q'){
            cin>>l>>r;
            cout<<range_query(l,r,1,n,0,root[t])<<endl;
        }else if(op=='H'){
            cin>>l>>r>>time;
            cout<<range_query(l,r,1,n,0,root[time])<<endl;
        }else if(op=='B'){
            cin>>t;
        }
    }
    return 0;
}