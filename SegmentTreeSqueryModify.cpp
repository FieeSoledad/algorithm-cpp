//
// Created by hsl on 2025/1/13.
//
#include<iostream>
#include<cmath>
using namespace std;
typedef long long ll;

const int MAXN = 100000+1;
ll diff_arr[MAXN];
ll range_sum[MAXN<<2];
ll range_max[MAXN<<2];
int n,m;

void up(int index){
    range_sum[index] = range_sum[index<<1]+range_sum[index<<1|1];
    range_max[index] = max(range_max[index<<1],range_max[index<<1|1]);
}

//构建线段树
void build(int left,int right,int index){
    if(left<right){
        int mid = (left+right)>>1;
        //构建子树
        build(left,mid,index<<1);
        build(mid+1,right,index<<1|1);
        //汇总信息
        up(index);
    }else if(left==right){
        range_sum[index] = diff_arr[left];
        range_max[index] = diff_arr[left];
    }
}

//开方
void square(int jobl,int jobr,int left,int right,int index){
    if(left==right){
        //TODO 当前元素
        ll sq = (ll)sqrt(range_sum[index]);
        range_sum[index] = sq;
        range_max[index] = sq;
    }else{
        int mid = (left+right)>>1;
        if(jobl<=mid && range_max[index<<1]>1){
            square(jobl,jobr,left,mid,index<<1);
        }//这里要剪枝
        if(jobr>mid && range_max[index<<1|1]>1){
            square(jobl,jobr,mid+1,right,index<<1|1);
        }
        up(index);
    }
}


/**
 * 查询区间和
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


//线段树上的特殊修改，区间开平方的操作。
//这个特殊修改没有懒更新
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    for(int idx=1;idx<=n;idx++){
        cin >> diff_arr[idx];
    }
    //建树
    build(1,n,1);
    cin>>m;
    int k,l,r;
    for(int idx=0;idx<m;idx++){
        cin>>k>>l>>r;
        if(l>r){//可能逆序
            int tmp = l;
            l = r;
            r = tmp;
        }
        if(k==0){
            //开方
            square(l,r,1,n,1);
        }else if(k==1){
            //查询区间和
            ll ans = query(l,r,1,n,1);
            cout<<ans<<endl;
        }
    }
    return 0;
}
