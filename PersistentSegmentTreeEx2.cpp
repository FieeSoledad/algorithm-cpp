//
// Created by hsl on 2025/4/3.
//
#include<iostream>
#include<algorithm> //sort
using namespace std;


const int MAXN = 2*1e5+5;
int n,m;
int arr[MAXN];
int sorted[MAXN];
int root[MAXN]; //各个版本线段树的头节点编号
int size[20*MAXN]; //当前位置代表的区间有多少个数
int left_[20*MAXN];
int right_[20*MAXN];
int s;//离散化后有多少个数字int cnt = 0;
int cnt = 0;

int build(int l,int r){
    int id = ++cnt;
    size[id] = 0;
    if(l<r){
        int mid = (l+r)>>1;
        left_[id] = build(l,mid);
        right_[id] = build(mid+1,r);
    }
    return id;
}

//二分查找排名
int kth(int num){//这是我的二分
    int left=1,right = s,mid,ans = 0;
    while(left<=right){
        mid = (left+right)>>1;
        if(sorted[mid]<num){
            ans = mid+1;
            left = mid+1;
        }else if(sorted[mid]==num){
            return mid;
        }else if(sorted[mid]>num){
            ans = mid-1;
            right = mid-1;
        }
    }
    return ans;
}

int insert(int jobi,int l,int r,int index){
    int id = ++cnt;//因为要生成新版本
    left_[id] = left_[index];
    right_[id] = right_[index];
    size[id] = size[index]+1;//新加了一个
    if(l<r){
        int mid = (l+r)>>1;
        if(jobi<=mid){
            left_[id] = insert(jobi,l,mid, left_[id]);
        }else{
            right_[id] = insert(jobi,mid+1,r,right_[id]);
        }
    }
    return id;
}

//权值做离散化并去重+生成各版本的线段树
void prepare(){
    for(int i=1;i<=n;i++) {
        sorted[i] = arr[i];
    }
    //排序
    sort(sorted+1,sorted+n+1);
    //去重
    s = 1;//这是末尾的指针
    for(int i=2;i<=n;i++){
        if(sorted[s]!=sorted[i]){
            sorted[++s] = sorted[i];
        }
    }
    root[0] = build(1,s);
    for(int i=1;i<=n;i++){
        int x = kth(arr[i]);
        root[i] = insert(x,1,s,root[i-1]);
    }
}


int query(int jobk,int l,int r,int u,int v){
    if(l==r){
        return l;
    }
    //TODO 这里
    int lsize = size[left_[v]]-size[left_[u]];
    int mid = (l+r)>>1;
    if(lsize>=jobk){
        return query(jobk,l,mid,left_[u],left_[v]);
    }else{
        return query(jobk-lsize,mid+1,r,right_[u],right_[v]);
    }
}
//https://www.luogu.com.cn/problem/P3834
//主席树
//单点修改+范围查询
//这是一个值域线段树
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    prepare();
    for(int i=1;i<=m;i++){
        int l,r,k;
        cin>>l>>r>>k;
        cout<<sorted[query(k,1,s,root[l-1],root[r])]<<endl;
    }
    return 0;
}
