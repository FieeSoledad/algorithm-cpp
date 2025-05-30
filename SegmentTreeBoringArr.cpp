//
// Created by hsl on 2025/1/14.
//
#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 100000+1;
int n;
int m;
int diff_arr[MAXN];
ll range_sum[MAXN<<2];
//TODO 居然是数据类型的错误
int add_step[MAXN<<2];

void up(int i){
    range_sum[i] =  range_sum[i<<1]+range_sum[i<<1|1];
}

void build(int l,int r,int i){
    if(l==r){
        range_sum[i] = diff_arr[l];
        add_step[i] = 0;
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
    }
}

void down(int lc,int rc,int i){
    if(add_step[i]!=0){
        range_sum[i<<1] += lc*add_step[i];
        range_sum[i<<1|1] += rc*add_step[i];
        //TODO 这里的懒信息应该是累积吧
        add_step[i<<1] += add_step[i];
        add_step[i<<1|1] += add_step[i];
        add_step[i] = 0;
    }
}

void range_add(int jobl,int jobr,int l,int r,int i,int v){
    if(jobl<=l && jobr>=r){
        //懒更新
        range_sum[i]+=(r-l+1)*v;
        add_step[i]+=v;
    }else{
        int mid = (l+r)>>1;
        down(mid- l+1,r-mid,i);
        if(jobl<=mid){
            range_add(jobl,jobr,l,mid,i<<1,v);
        }
        if(jobr>mid){
            range_add(jobl,jobr,mid+1,r,i<<1|1,v);
        }
        up(i);
    }
}

ll query(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return range_sum[i];
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        ll ans = 0;
        if(jobl<=mid){
            ans+= query(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans+= query(jobl,jobr,mid+1,r,i<<1|1);
        }
        return ans;
    }
}

//https://www.luogu.com.cn/problem/P1438
//洛谷无聊的数列
//维护一个数列
//利用差分数组表示数
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m;
    cin >> diff_arr[1];
    //TODO 居然是差分数组构建错误
    int pre =diff_arr[1];
    for(int idx=2;idx<=n;idx++){
        int cur;
        cin>>cur;
        diff_arr[idx] = cur- pre;
        pre = cur;
    }
    build(1,n,1);
    int op,l,r,K,D,p;
    for(int idx=0;idx<m;idx++){
        cin>>op;
        if(op==1){
            cin>>l>>r>>K>>D;
            range_add(l,l,1,n,1,K);
            //TODO 是否有必要执行
            if(l+1<=r){
                range_add(l+1,r,1,n,1,D);
            }
            if(r<n){
                range_add(r+1,r+1,1,n,1,0-(K+(r-l+1-1)*D));
            }
        }else if(op==2){
            cin>>p;
            ll ans = query(1,p,1,n,1);
            cout<<ans<<endl;
        }
    }
    return 0;
}
