//
// Created by hsl on 2025/1/17.
//

#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 1e9+1;
const int MAXM = 1000;
const int LIMIT = 80001;
int cnt=1;
int n,m;
int left_node[LIMIT];
int right_node[LIMIT];
ll range_sum[LIMIT];
ll add_step[LIMIT];

void up(int cur){
    range_sum[cur] = range_sum[left_node[cur]]+range_sum[right_node[cur]];
}

void lazy_add(int cnt,int i,int v){
    range_sum[i]+=cnt*v;
    add_step[i]+=v;
}

//懒信息下发
void down(int lc,int rc,ll i){
    //没有懒信息
    if(add_step[i]==0){
        return;
    }
    if(left_node[i]==0){
        left_node[i] = ++cnt;
    }
    if(right_node[i]==0){
        right_node[i] = ++cnt;
    }
    lazy_add(lc,left_node[i],add_step[i]);
    lazy_add(rc,right_node[i],add_step[i]);
    //TODO 错误1
    add_step[i] = 0;
}


void range_add(int jobl,int jobr,int l,int r,int i,ll v){
    if(jobl<=l && jobr>=r){
        lazy_add(r-l+1,i,v);
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        if(jobl<=mid){
            if(left_node[i]==0){
                left_node[i] = ++cnt;
            }
            range_add(jobl,jobr,l,mid,left_node[i],v);
        }
        if(jobr>mid){
            if(right_node[i]==0){
                right_node[i] = ++cnt;
            }
            range_add(jobl,jobr,mid+1,r,right_node[i],v);
        }
        up(i);
    }
}


//查询期间遇见空直接返回0
ll query(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return range_sum[i];
    }else{
        int mid = (l+r)>>1;
        //懒信息下推
        down(mid-l+1,r-mid,i);
        ll ans = 0;
        if(jobl<=mid){
            if(left_node[i]!=0){
                ans+= query(jobl,jobr,l,mid,left_node[i]);
            }
        }
        if(jobr>mid){
            if(right_node[i]!=0){
                ans+= query(jobl,jobr,mid+1,r,right_node[i]);
            }
        }
        return ans;
    }
}

//https://www.luogu.com.cn/problem/P2781
//泉岭精神传教
//省空间的开点线段树
//题目有问题，不是说的k<=10？
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m;
    cnt = 1;
    int op,l,r;
    //TODO 居然是这个数据类型的问题
    ll k;
    for(int idx=0;idx<m;idx++){
        cin>>op;
        if(op==1){
            cin>>l>>r>>k;
            range_add(l,r,1,n,1,k);
        }else if(op==2){
            cin>>l>>r;
            ll ans = query(l,r,1,n,1);
            cout<<ans<<endl;
        }
    }
    return 0;
}