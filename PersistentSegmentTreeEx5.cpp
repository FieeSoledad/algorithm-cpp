//
// Created by hsl on 2025/4/6.
//
#include<iostream>
#include<cstring>
using namespace std;

const int MAXN = 2*1e5+2;
const int MAXT = MAXN*10;
int cases,n,m;
int arr[MAXN];
int root[MAXN]; //主席树的实现
int ln[MAXT];
int rn[MAXT];
int first_size[MAXT];
//int add[MAXT];
int last_occur[MAXN];
int lastAns = 0;
int cnt = 0;

void clear(){
    memset(arr,0,sizeof(arr));
    memset(root,0,sizeof(root));
    memset(ln,0,sizeof(ln));
    memset(rn,0,sizeof(rn));
    memset(first_size,0,sizeof(first_size));
    memset(last_occur,0,sizeof(last_occur));
    cnt = 0;
    lastAns = 0;
}

void up(int i){
    first_size[i] = first_size[ln[i]]+first_size[rn[i]];
}

int build(int l,int r){
    int id = ++cnt;
    if(l==r){
        first_size[id] = 0;
    }else{
        int mid = (l+r)>>1;
        ln[id] = build(l,mid);
        rn[id] = build(mid+1,r);
        first_size[id] = first_size[ln[id]]+first_size[rn[id]];
//        up(id);
    }
    return id;
}

int clone(int i){
    int id = ++cnt;
    ln[id] = ln[i];
    rn[id] = rn[i];
    first_size[id] = first_size[i];
    return id;
}

int range_add(int jobi,int l,int r,int value,int i){//设置为0或1
    i = clone(i);
    if(l==r){
        first_size[i]+=value;
    }else{
        int mid = (l+r)>>1;
        if(jobi<=mid){
            ln[i] = range_add(jobi,l,mid,value,ln[i]);
        }
        if(jobi>mid){
            rn[i] = range_add(jobi,mid+1,r,value,rn[i]);
        }
        //TODO 没有汇总信息?
        up(i);
    }
    return i;
}

int range_query_first(int jobl,int jobr,int l,int r,int i){//查询第几个
    if(jobl<=l && jobr>=r){
        return first_size[i];
    }else{
        int mid = (l+r)>>1;
        int ans = 0;
        if(jobl<=mid){
            ans+= range_query_first(jobl,jobr,l,mid,ln[i]);
        }
        if(jobr>mid){
            ans+= range_query_first(jobl,jobr,mid+1,r,rn[i]);
        }
        return ans;
    }
}

int query_kth(int l,int r,int kth,int i){
    if(l==r){
        //TODO 设置为
//        return arr[l];
        return l;
    }
    int mid = (l+r)>>1;
    int lcnt = first_size[ln[i]];
    if(lcnt>=kth){
        return query_kth(l,mid,kth,ln[i]);
    }
    if(kth>lcnt){
        return query_kth(mid+1,r,kth-lcnt,rn[i]);
    }
}

void prepare(){
    for(int i=n;i>=1;i--){
        if(last_occur[arr[i]]==0){//上一个
            last_occur[arr[i]] = i;
            root[i] = range_add(i,1,n,1,root[i+1]);
        }else{
            root[i] = range_add(last_occur[arr[i]],1,n,-1,root[i+1]);
            //TODO 忘记设1了
            root[i] = range_add(i,1,n,1,root[i]);
            last_occur[arr[i]] = i;
        }
    }
}

//维护第一次出现的数字的位置
//https://acm.hdu.edu.cn/showproblem.php?pid=5919
//突然想到整数除法上取整怎么做呢？不知道呢
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>cases;
    int a,b;
    for(int idx=1;idx<=cases;idx++){

        clear();
        cin>>n>>m;
        for(int i=1;i<=n;i++){
            cin>>arr[i];
        }
        root[n+1] = build(1,n);//初始建树
        prepare();
        int l,r;
        cout<<"Case #"<<idx<<":";
        for(int i=1;i<=m;i++){
            cin>>l>>r;
            //TODO 看清题意
            a = min((l+lastAns)%n+1,(r+lastAns)%n+1);
            b = max((l+lastAns)%n+1,(r+lastAns)%n+1);

            int lcnt = range_query_first(a,b,1,n,root[a]);
//            int rcnt = range_query_first(b+1,n,1,n,root[b+1]);
            int rcnt = 0;
            int target = (lcnt-rcnt+1)/2;
            lastAns = query_kth(1,n,target,root[a]);
            cout<<" "<<lastAns;
        }
        cout<<endl;
    }
    return 0;
}
