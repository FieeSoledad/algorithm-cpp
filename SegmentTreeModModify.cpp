//
// Created by hsl on 2025/1/13.
//

#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 100000+1;
int diff_arr[MAXN];
ll range_sum[MAXN<<2];
ll range_max[MAXN<<2];
int n,m;

void up(int index){
    range_sum[index] = range_sum[index<<1] + range_sum[index<<1|1];
    range_max[index] = max(range_max[index<<1],range_max[index<<1|1]);
}

void build(int left,int right,int index){
    if(left==right){
        range_sum[index] = diff_arr[left];
        range_max[index] = diff_arr[left];
    }else{
        int mid = (left+right)>>1;
        build(left,mid,index<<1);
        build(mid+1,right,index<<1|1);
        up(index);
    }
}

/**
 * 查询区间累加和
 * @param jobl
 * @param jobr
 * @param left
 * @param right
 * @param index
 * @return
 */
ll query(int jobl,int jobr,int left,int right,int index){
    if(jobl<=left && jobr>=right){
        return range_sum[index];
    }else{
        int mid = (left+right)>>1;
        ll ans = 0;
        if(jobl<=mid){
            ans+= query(jobl,jobr,left,mid,index<<1);
        }
        if(jobr>mid){
            ans+= query(jobl,jobr,mid+1,right,index<<1|1);
        }
        return ans;
    }
}

//线段树上的取模操作
void mod_op(int jobl,int jobr,int left,int right,int index,int m){
    if(left==right){
        ll res = range_sum[index] % m;
        range_sum[index] = res;
        range_max[index] = res;
    }else{
        int mid = (left+right)>>1;
        //什么时候可以不用再取模了呢？
        if(jobl<=mid && range_max[index<<1]>=m){
            mod_op(jobl,jobr,left,mid,index<<1,m);
        }
        //TODO 之前没有或1
        if(jobr>mid && range_max[index<<1|1]>=m){
            mod_op(jobl,jobr,mid+1,right,index<<1|1,m);
        }
        //汇总信息
        up(index);
    }
}

/**
 * 区间重置操作
 * @param jobl
 * @param jobr
 * @param left
 * @param right
 * @param index
 * @param value
 */
void update_op(int jobl,int jobr,int left,int right,int index,int value){
    if(left==right){
        range_sum[index] = value;
        range_max[index] = value;
    }else{
        int mid = (left+right)>>1;
        if(jobl<=mid){
            update_op(jobl,jobr,left,mid,index<<1,value);
        }
        if(jobr>mid){
            update_op(jobl,jobr,mid+1,right,index<<1|1,value);
        }
        //汇总信息
        up(index);
    }
}

//https://www.luogu.com.cn/problem/CF438D
//线段树上不断做取模操作
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m;
    for(int idx=1;idx<=n;idx++){
        cin >> diff_arr[idx];
    }

    //建树
    build(1,n,1);

    int op,l,r,x,k;
    for(int idx=0;idx<m;idx++){
        cin>>op;
        if(op==1){//求和
            cin>>l>>r;
            ll ans = query(l,r,1,n,1);
            cout<<ans<<endl;
        }else if(op==2){//取模
            cin>>l>>r>>x;
            mod_op(l,r,1,n,1,x);
        }else if(op==3){//重置
            cin>>k>>x;
            update_op(k,k,1,n,1,x);
        }
    }
    return 0;
}
