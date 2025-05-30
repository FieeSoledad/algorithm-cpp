//
// Created by hsl on 2025/1/18.
//
#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 1000000+1;
int LOWEST = -1;
int arr[MAXN];
ll range_sum[MAXN<<2];
int max_[MAXN<<2];
int cnt[MAXN<<2];
int sem[MAXN<<2];
int n,m;

void up(int i){
    int l = i<<1;
    int r = i<<1|1;
    range_sum[i] = range_sum[l]+range_sum[r];
    max_[i] = max(max_[l],max_[r]);
    if(max_[l]>max_[r]){
        cnt[i]=  cnt[l];
        sem[i] = max(sem[l],max_[r]);
    }else if(max_[l]<max_[r]){
        cnt[i] = cnt[r];
        sem[i] = max(sem[r],max_[l]);
    }else{
        cnt[i] = cnt[l]+cnt[r];
        sem[i] = max(sem[l],sem[r]);
    }
}

void lazy(int i,int v){
    //为什么需要if语句?在懒信息下推时有用，父节点的最大值只来自于其中一部分
    if(v<max_[i]){
        //TODO 这里有数据溢出的问题
        range_sum[i]-=((ll)max_[i]-v)*cnt[i];
        max_[i] = v;
    }
}

void down(int i){
    lazy(i<<1,max_[i]);
    lazy(i<<1|1,max_[i]);
}

//设置上限
void set_ceiling(int jobl,int jobr,int l,int r,int i,int v){
    if(v>=max_[i]){
        //所有数都不会发生变化
        return;
    }
    //TODO 这里用大于等于没有关系
    if(jobl<=l && jobr>=r && v>=sem[i]){
        lazy(i,v);
    }else{
        down(i);
        int mid = (l+r)>>1;
        if(jobl<=mid){
            set_ceiling(jobl,jobr,l,mid,i<<1,v);
        }
        if(jobr>mid){
            set_ceiling(jobl,jobr,mid+1,r,i<<1|1,v);
        }
        up(i);
    }
}

int query_max(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return max_[i];
    }else{
        down(i);
        int mid = (l+r)>>1;
        int max1 = 0,max2 = 0;
        if(jobl<=mid){
            max1 = query_max(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            max2 = query_max(jobl,jobr,mid+1,r,i<<1|1);
        }
        return max(max1,max2);
    }
}

ll query_sum(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return range_sum[i];
    }else{
        down(i);
        int mid = (l+r)>>1;
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

//设限操作要怎么懒呢
//现在是要怎么发生懒信息呢？Codeforces说实话
void build(int l,int r,int i){
    if(l==r){
        range_sum[i] = arr[l];
        max_[i] = arr[l];
        cnt[i] = 1;
        sem[i] = LOWEST;
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
    }
}

//https://acm.hdu.edu.cn/showproblem.php?pid=5306
//区间设限
//吉如一线段树

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin>>T;
    int op,x,y,t;
    while(T--!=0){
        cin>>n>>m;
        for(int idx=1;idx<=n;idx++){
            cin>>arr[idx];
        }
        build(1,n,1);
        for(int idx=0;idx<m;idx++){
            cin>>op>>x>>y;
            if(op==0){//replace
                cin>>t;
                set_ceiling(x,y,1,n,1,t);
            }else if(op==1){//maximum
                int ans = query_max(x,y,1,n,1);
                cout<<ans<<endl;
            }else if(op==2){//sum
                ll ans = query_sum(x,y,1,n,1);
                cout<<ans<<endl;
            }
        }
    }
    return 0;
}
