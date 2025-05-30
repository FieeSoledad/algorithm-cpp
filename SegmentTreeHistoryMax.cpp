//
// Created by hsl on 2025/1/18.
//
#include<iostream>
#include<climits>
using namespace std;
typedef long long ll;

const int MAXN = 5*100000+1;
const ll LOWEST = LONG_MIN;
int arr[MAXN];
ll range_sum[MAXN<<2];
ll max_[MAXN<<2];
int cnt[MAXN<<2];
ll sem[MAXN<<2];
//新增的懒信息
//这样设置懒信息是不行的
ll max_add[MAXN<<2];
ll other_add[MAXN<<2];
//历史最值
ll max_history[MAXN<<2];
//历史最大提升幅度。反正本来就是0
ll max_add_top[MAXN<<2];
ll other_add_top[MAXN<<2];
int n,m;

void up(int i){
    int l = i<<1;
    int r = i<<1|1;
    range_sum[i] = range_sum[l]+range_sum[r];
    //TODO
    max_history[i] = max(max_history[l],max_history[r]);
    max_[i] = max(max_[l],max_[r]);
    if(max_[l]>max_[r]){
        cnt[i] = cnt[l];
        sem[i] = max(sem[l],max_[r]);
    }else if(max_[l]<max_[r]){
        cnt[i] = cnt[r];
        sem[i] = max(sem[r],max_[l]);
    }else{
        cnt[i] = cnt[l]+cnt[r];
        sem[i] = max(sem[l],sem[r]);
    }
}


void build(int l,int r,int i){
    if(l==r){
        range_sum[i] = arr[l];
        max_[i] = arr[l];
        cnt[i] = 1;
        sem[i] = LOWEST;
        max_add[i] = 0;
        other_add[i] = 0;
        max_history[i] = arr[l];
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
        max_add[i] = 0;
        other_add[i] = 0;
    }
}

//多出两个参数是为了方便down
//假设已经有的历史
void lazy(int n,int i,ll maxAdd,ll otherAdd,ll maxUpv,ll otherUpv){
    max_history[i] = max(max_history[i],max_[i]+maxUpv);
    max_add_top[i] = max(max_add_top[i],max_add[i]+maxUpv);
    other_add_top[i] = max(other_add_top[i],other_add[i]+otherUpv);

    range_sum[i]+=(ll)cnt[i]*maxAdd+otherAdd*(n-cnt[i]);
    max_[i] += maxAdd;
    sem[i] += (sem[i]==LOWEST)?0:otherAdd;
    max_add[i]+= maxAdd;
    other_add[i]+=otherAdd;
}

//现在也是有两种操作，是否互斥呢？
//一开始设置的信息设置不当
void down(int lc,int rc,int i){
    int l = i<<1;
    int r = i<<1|1;
    ll temp = max(max_[l],max_[r]);
    if(temp==max_[l]){
        lazy(lc,l,max_add[i],other_add[i],max_add_top[i],other_add_top[i]);
    }else{
        lazy( lc,l,other_add[i],other_add[i],other_add_top[i],other_add_top[i]);
    }
    if(temp==max_[r]){
        lazy(rc,r,max_add[i],other_add[i],max_add_top[i],other_add_top[i]);
    }else{
        lazy(rc,r,other_add[i],other_add[i],other_add_top[i],other_add_top[i]);
    }
    max_add[i] = 0;
    other_add[i] = 0;
    max_add_top[i] = 0;
    other_add_top[i] = 0;
}



void range_add(int jobl,int jobr,int l,int r,int i,ll v){
    if(jobl<=l && jobr>=r){
        int cnt = r-l+1;
        lazy(cnt,i,v,v,v,v);
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        if(jobl<=mid){
            range_add(jobl,jobr,l,mid,i<<1,v);
        }
        if(jobr>mid){
            range_add(jobl,jobr,mid+1,r,i<<1|1,v);
        }
        up(i);
    }
}


void set_ceiling(int jobl,int jobr,int l,int r,int i,ll v){
    if(max_[i]<=v){
        return;
    }
    if(jobl<=l && jobr>=r && v>=sem[i]){
        //TODO 第一个错误
        lazy(r-l+1,i,v-max_[i],0,v-max_[i],0);
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        if(jobl<=mid){
            set_ceiling(jobl,jobr,l,mid,i<<1,v);
        }
        if(jobr>mid){
            set_ceiling(jobl,jobr,mid+1,r,i<<1|1,v);
        }
        up(i);
    }
}

ll query_sum(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return range_sum[i];
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        ll ans = 0;
        if(jobl<=mid){
            ans+= query_sum(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans+= query_sum(jobl,jobr,mid+1,r,i<<1|1);
        }
        return ans;
    }
}

ll query_max(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return max_[i];
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        ll ans1 = LOWEST;
        if(jobl<=mid){
            ans1 = query_max(jobl,jobr,l,mid,i<<1);
        }
        ll ans2 = LOWEST;
        if(jobr>mid){
            ans2 = query_max(jobl,jobr,mid+1,r,i<<1|1);
        }
        return max(ans1,ans2);
    }
}

ll query_history_max(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return max_history[i];
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        ll ans1 = LOWEST,ans2 = LOWEST;
        if(jobl<=mid){
            ans1 = query_history_max(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans2 = query_history_max(jobl,jobr,mid+1,r,i<<1|1);
        }
        return max(ans1,ans2);
    }
}

//https://www.luogu.com.cn/problem/P6242
//区间历史最值问题，问题是历史最值在down时如何下发。积累即可，笑死了
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int idx=1;idx<=n;idx++){
        cin>>arr[idx];
    }
    build(1,n,1);
    int op,l,r;
    ll k,v;
    for(int idx=0;idx<m;idx++){
        cin>>op>>l>>r;
        if(op==1){//add
            cin>>k;
            range_add(l,r,1,n,1,k);
        }else if(op==2){//set_ceiling
            cin>>v;
            set_ceiling(l,r,1,n,1,v);
        }else if(op==3){//query_sum
            ll ans = query_sum(l,r,1,n,1);
            cout<<ans<<endl;
        }else if(op==4){//query_max
            ll ans = query_max(l,r,1,n,1);
            cout<<ans<<endl;
        }else if(op==5){//query_history_max
            ll ans = query_history_max(l,r,1,n,1);
            cout<<ans<<endl;
        }
    }
    return 0;
}