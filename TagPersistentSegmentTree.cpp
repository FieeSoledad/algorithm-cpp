//
// Created by hsl on 2025/4/6.
//
#include<iostream>
using namespace std;
typedef long long ll;
const int MAXN = 100001;
//TODO 我真的
ll arr[MAXN];
ll sum[MAXN<<2];
ll add_tag[MAXN<<2];//标记永久化
int n,m;

void up(int i){
    sum[i] = sum[i<<1]+sum[i<<1|1];
}

void build(int l,int r,int i){
    if(l==r){
        sum[i]=arr[l];
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
    }
    add_tag[i] = 0;
}

//不这样的话修改子区间查询父区间会出错
//void range_add(int jobl,int jobr,int l,int r,ll value,int i){
//    int a = max(jobl,l);
//    int b = min(jobr,r);
//    sum[i]+=(ll)(b-a+1)*value;
//    if(jobl<=l && jobr>=r){
//        add_tag[i]+=value;
//    }else{
//        int mid = (l+r)>>1;
//        if(jobl<=mid){
//            range_add(jobl,jobr,l,mid,value,i<<1);
//        }
//        if(jobr>mid){
//            range_add(jobl,jobr,mid+1,r,value,i<<1|1);
//        }
//    }
//}

void add(int jobl, int jobr,int l, int r, ll jobv, int i) {
    int a = max(jobl, l), b = min(jobr, r);
    sum[i] += jobv * (b - a + 1);
    if (jobl <= l && r <= jobr) {
        add_tag[i] += jobv;
    } else {
        int mid = (l + r) / 2;
        if (jobl <= mid) {
            add(jobl, jobr,  l, mid,jobv, i << 1);
        }
        if (jobr > mid) {
            add(jobl, jobr, mid + 1, r,jobv, i << 1 | 1);
        }
    }
}

//ll range_query(int jobl,int jobr,int l,int r,ll his_tag,int i){
//    if(jobl<=l && jobr>=r){
//        int cnt = r-l+1;
//        ll ans = sum[i]+(ll)cnt*(his_tag);
//        return ans;
//    }else{
//        int mid = (l+r)>>1;
//        ll ans = 0;
//        if(jobl<=mid){
//            ans+=range_query(jobl,jobr,l,mid,his_tag+add_tag[i],i<<1);
//        }
//        if(jobr>mid){
//            ans+=range_query(jobl,jobr,mid+1,r,his_tag+add_tag[i],i<<1|1);
//        }
//        return ans;
//    }
//}

long long query(int jobl, int jobr, int l, int r, ll his_tag,int i) {
    if (jobl <= l && r <= jobr) {
        return sum[i] + his_tag * (r - l + 1);
    }
    int mid = (l + r) >> 1;
    long long ans = 0;
    if (jobl <= mid) {
        ans += query(jobl, jobr, l, mid,his_tag + add_tag[i],  i << 1);
    }
    if (jobr > mid) {
        ans += query(jobl, jobr,  mid + 1, r, his_tag + add_tag[i],i << 1 | 1);
    }
    return ans;
}


//https://www.luogu.com.cn/problem/P3372
//翻译题目的我真的cnm
//使用标记永久化优化空间占用
//重点：没有懒信息下发环节,也没有up，所以sum信息应该变化
//这里sum信息定义为什么呢？
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    build(1,n,1);
    for(int i=1;i<=m;i++){
        int op,x,y;
        //TODO 我真的
        ll k;
        cin>>op>>x>>y;
        if(op==1){
            cin>>k;
            add(x,y,1,n,k,1);
        }else if(op==2){
            cout<<query(x,y,1,n,0,1)<<endl;
        }
    }
    return 0;
}
