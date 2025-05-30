//
// Created by hsl on 2025/1/16.
//

#include<iostream>
using namespace std;

const int MAXN = 100000+1;
int arr[MAXN];
int range_sum[MAXN<<2];

int len1[MAXN<<2];
int pre1[MAXN<<2];
int suf1[MAXN<<2];

int len0[MAXN<<2];
int pre0[MAXN<<2];
int suf0[MAXN<<2];

bool all1[MAXN<<2];
bool all0[MAXN<<2];
bool rev[MAXN<<2];
//重置值的懒信息
short int set[MAXN<<2];
int n,m;

void up(int i){
    int l = i<<1;
    int r = i<<1|1;
    range_sum[i] = range_sum[l]+range_sum[r];

    len0[i] = max(max(len0[l],len0[r]),suf0[l]+pre0[r]);
    pre0[i] = all0[l]?pre0[l]+pre0[r]:pre0[l];
    suf0[i] = all0[r]?suf0[r]+suf0[l]:suf0[r];

    len1[i] = max(max(len1[l],len1[r]),suf1[l]+pre1[r]);
    pre1[i] = all1[l]?pre1[l]+pre1[r]:pre1[l];
    suf1[i] = all1[r]?suf1[r]+suf1[l]:suf1[r];

    all0[i] = (all0[l] && all0[r]);
    all1[i] = (all1[l] && all1[r]);
}


void build(int l,int r,int i){
    if(l==r){
        range_sum[i] = arr[l];

        len1[i] = arr[l];
        pre1[i] = arr[l];
        suf1[i] = arr[l];

        //TODO 初始值
        len0[i] = 1-arr[l];
        pre0[i] = 1-arr[l];
        suf0[i] = 1-arr[l];

        rev[i] = false;
        set[i] = 2;
        all1[i] = (arr[l]==1);
        all0[i] = (arr[l]==0);
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
        //TODO 非叶节点的懒信息
        rev[i] = false;
        set[i] = 2;
    }
}

void lazy_set(int cnt,int i,int v){
    if(v==0){
        range_sum[i] = 0;
        len0[i] = cnt;
        pre0[i] = cnt;
        suf0[i] = cnt;

        len1[i] = 0;
        pre1[i] = 0;
        suf1[i] = 0;

        all0[i] = true;
        all1[i] = false;
        set[i] = 0;

        //翻转信息被覆盖了
        rev[i] = false;
    }else{
        range_sum[i] = cnt;
        len0[i] = 0;
        pre0[i] = 0;
        suf0[i] = 0;

        len1[i] = cnt;
        pre1[i] = cnt;
        suf1[i] = cnt;

        all0[i] = false;
        all1[i] = true;
        set[i] = 1;
        //翻转信息被覆盖了
        rev[i] = false;
    }
}

void swap(int* arr1,int* arr2,int index1,int index2){
    int temp = arr1[index1];
    arr1[index1] = arr2[index2];
    arr2[index2] = temp;
}

void lazy_rev(int cnt,int i){
    range_sum[i] = cnt-range_sum[i];
    swap(len1,len0,i,i);
    swap(pre1,pre0,i,i);
    swap(suf1,suf0,i,i);
    bool tmp = all0[i];
    all0[i] = all1[i];
    all1[i] = tmp;
    //有懒信息
    //TODO 这个翻转要特别注意诶
    rev[i] = !rev[i];
}

//这里有两种操作会导致出现懒信息，这个该如何处理？
void down(int lc,int rc,int i){
    if(set[i]!=2){
        lazy_set(lc,i<<1,set[i]);
        lazy_set(rc,i<<1|1,set[i]);
        //没有懒信息了
        set[i] = 2;
    }
    if(rev[i]){
        lazy_rev(lc,i<<1);
        lazy_rev(rc,i<<1|1);
        rev[i] = false;
    }
}


//设置值的懒信息如何下发呢？
void range_set(int jobl,int jobr,int l,int r,int i,int v){
    if(jobl<=l && jobr>=r){
        int cnt = r-l+1;
        lazy_set(cnt,i,v);
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        if(jobl<=mid){
            range_set(jobl,jobr,l,mid,i<<1,v);
        }
        if(jobr>mid){
            range_set(jobl,jobr,mid+1,r,i<<1|1,v);
        }
        up(i);
    }
}

void range_rev(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        //TODO 计算
        int cnt = r-l+1;
        lazy_rev(cnt,i);
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        if(jobl<=mid){
            range_rev(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            range_rev(jobl,jobr,mid+1,r,i<<1|1);
        }
        up(i);
    }
}

int query_sum(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return range_sum[i];
    }else{
        int mid = (l+r)>>1;
        //TODO 查询时懒信息下推
        down(mid-l+1,r-mid,i);
        int ans = 0;
        if(jobl<=mid){
            ans+= query_sum(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans+= query_sum(jobl,jobr,mid+1,r,i<<1|1);
        }
        return ans;
    }
}

//type: 0: 最长连续的1
//type: 1: 前缀
//type: 2: 后缀
int query_inf(int jobl,int jobr,int l,int r,int i,int type){
    if(jobl<=l && jobr>=r){
        if(type==0){
            return len1[i];
        }else if(type==1){//查前缀
            return pre1[i];
        }else if(type==2){//查后缀
            return suf1[i];
        }
    }else if(type==0){//查询最长连续的1
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);

        if(jobl>mid){
            return query_inf(jobl,jobr,mid+1,r,i<<1|1,0);
        }
        if(jobr<=mid){
            return query_inf(jobl,jobr,l,mid,i<<1,0);
        }

        int l1 = 0,l2 = 0;
        int p = 0,s = 0;

        //左区间的后缀
        s = query_inf(jobl,mid,l,mid,i<<1,2);
        //左区间的长度，这是因为后续可能出现调用获取前缀和后缀的情况，为了避免边界紊乱。
        l1 = query_inf(jobl,mid,l,mid,i<<1,0);

        //右区间的前缀
        p = query_inf(mid+1,jobr,mid+1,r,i<<1|1,1);
        l2 = query_inf(mid+1,jobr,mid+1,r,i<<1|1,0);

        return max(max(l1,l2),s+p);
    }else if(type==1){//查询前缀
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);

        if(jobl>mid){
            return query_inf(jobl,jobr,mid+1,r,i<<1|1,1);
        }
        if(jobr<=mid){
            return query_inf(jobl,jobr,l,mid,i<<1,1);
        }

        int p1 = 0,p2 = 0;
        //左区间的前缀
        p1 = query_inf(jobl,mid,l,mid,i<<1,1);
        //右区间的前缀
        p2 = query_inf(mid+1,jobr,mid+1,r,i<<1|1,1);

        //左区间的长度
        int num = mid-jobl+1;
        int s = query_sum(jobl,jobr,l,mid,i<<1);
        //这个信息整合比较难写诶。
        return s==num? (p1+p2):p1;
    }else if(type==2){//查询后缀
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);

        if(jobl>mid){
            return query_inf(jobl,jobr,mid+1,r,i<<1|1,2);
        }

        if(jobr<=mid){
            return query_inf(jobl,jobr,l,mid,i<<1,2);
        }

        int s1 = 0,s2 = 0;
        //左区间的后缀
        s1 = query_inf(jobl,mid,l,mid,i<<1,2);
        //右区间的后缀
        s2 = query_inf(mid+1,jobr,mid+1,r,i<<1|1,2);

        int num = jobr-mid;
        int s = query_sum(jobl,jobr,mid+1,r,i<<1|1);
        return s==num?s1+s2:s2;
    }
}


//翻转值的懒信息
//https://www.luogu.com.cn/problem/P2572
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m;
    for(int idx=1;idx<=n;idx++){
        cin>>arr[idx];
    }
    build(1,n,1);
    int op,l,r;
    for(int idx=0;idx<m;idx++) {
        cin >> op >> l >> r;
        //TODO：下标从0开始!
        l+=1;
        r+=1;
        if (op == 0) {
            range_set(l, r, 1, n, 1, 0);
        } else if (op == 1) {
            //这里逻辑就有问题 v吧
            range_set(l, r, 1, n, 1, 1);
        } else if (op == 2) {
            range_rev(l, r, 1, n, 1);
        } else if (op == 3) {
            int ans = query_sum(l, r, 1, n, 1);
            cout << ans << endl;
        } else if (op == 4) {
            //一个查询方法为什么会影响到结果？=
            int ans = query_inf(l, r, 1, n, 1,0);
            cout << ans << endl;
        }
        int t = 0;
    }
    int t = 0;
    return 0;
}
