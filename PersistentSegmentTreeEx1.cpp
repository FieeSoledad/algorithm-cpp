//
// Created by hsl on 2025/4/3.
//
#include<iostream>
using namespace std;

int n,m;

const int MAXN = 1e6+1;
const int MAXT = MAXN*23; //果然是数据空间的问题诶。
int arr[MAXN];
int root[MAXN];
int left_[MAXT];
int right_[MAXT];
int value[MAXT];
int cnt = 0;//

//建树，返回头节点编号
int build(int l,int r){
    int id = ++cnt;//空间编号自增
    if(l==r){
        value[id] = arr[l];
    }else{
        int mid = (l+r)>>1;
        left_[id] = build(l,mid);
        right_[id] = build(mid+1,r);
    }
    return id;
}

int update(int jobi,int jobv,int l,int r,int head){//修改，返回头节点的编号
    int cur = ++cnt;
    left_[cur] = left_[head];
    right_[cur] = right_[head];
    value[cur] = value[head];
    if(l==r){
        value[cur] = jobv;
    }else{
        int mid = (l+r)>>1;
        if(jobi<=mid){
            left_[cur]= update(jobi,jobv,l,mid,left_[cur]);
        }else{
            right_[cur] = update(jobi,jobv,mid+1,r,right_[cur]);
        }
    }
    return cur;
}

int query(int jobi,int l,int r,int head){//查询
    if(l==r){
        return value[head];
    }else{
        int mid = (l+r)>>1;
        if(jobi<=mid){
            return query(jobi,l,mid,left_[head]);
        }else{
            return query(jobi,mid+1,r,right_[head]);
        }
    }
}


//https://www.luogu.com.cn/problem/P3919
//思考：这题好像没有特别需要线段树那样维护的信息
//这里是单点修改+单点查询
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    root[0] = build(1,n);
    for(int i=1,version,op,x,y;i<=m;i++){
        cin>>version;
        cin>>op;
        if(op==1){ //修改
            cin>>x>>y;
            root[i] = update(x,y,1,n,root[version]);
        }else{
            cin>>x;
            root[i] = root[version];
            cout<<query(x,1,n,root[i])<<endl;
        }
    }
    return 0;
}
