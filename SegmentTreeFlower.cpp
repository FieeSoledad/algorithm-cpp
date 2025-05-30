//
// Created by hsl on 2025/1/13.
//
#include<iostream>
#include<vector>
using namespace std;

const int MAXN = 50001;
//这个线段树只需要维护区间和
int range_sum[MAXN<<2];
//需要支持一个区间设置值操作
int change[MAXN<<2];
//是否有懒信息，区间修改这个是。
bool update[MAXN<<2];
int T,N,M;

//构建线段树
void build(int left,int right,int index){
    if(left<right){
        int mid = (left+right)>>1;
        build(left,mid,index<<1);
        build(mid+1,right,index<<1|1);
    }
    range_sum[index] = 0;
    update[index] = false;
}

void lazy_change(int index,int count,int value){
    range_sum[index] = count*value;
    change[index] = value;
    update[index] = true;
}

void up(int index){
    range_sum[index] = range_sum[index<<1]+range_sum[index<<1|1];
}

//懒信息下推需要依据区间大小
void down(int index,int lc,int rc){
    //懒信息下推
    if(update[index]){
        lazy_change(index<<1,lc,change[index]);
        lazy_change(index<<1|1,rc,change[index]);
        update[index] = false;
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
int query(int jobl,int jobr,int left,int right,int index){
    if(jobl<=left && jobr>=right){
        return range_sum[index];
    }else{
        int mid = (left+right)/2;
        down(index,mid-left+1,right-mid);
        int ans = 0;
        if(jobl<=mid){
            ans+=query(jobl,jobr,left,mid,index<<1);
        }
        if(jobr>mid){
            ans+=query(jobl,jobr,mid+1,right,index<<1|1);
        }
        return ans;
    }
}

/**
 * 查询数组从start开始第k个0
 * 采用二分搜索
 * @param start
 * @param k
 * @return
 */
int find_zero(int start,int k){
    int l = start,r = N,mid;
    int ans = 0;
    while(l<=r){
        mid = (l+r)>>1;
        //TODO 有空缺，需要特殊处理
        if(mid-start+1-query(start,mid,1,N,1)>=k){
            ans = mid;
            r = mid-1;
        }else if(mid-start+1-query(start,mid,1,N,1)<k){
            l = mid+1;
        }
    }
    return ans;
}

//支持区间修改
void update_value(int jobl,int jobr,int left,int right,int index,int value){
    if(jobl<=left && jobr>=right){
        lazy_change(index,right-left+1,value);
    }else{
        int mid = (left+right)>>1;
        down(index,mid-left+1,right-mid);
        if(jobl<=mid){
            update_value(jobl,jobr,left,mid,index<<1,value);
        }
        if(jobr>mid){
            update_value(jobl,jobr,mid+1,right,index<<1|1,value);
        }
        //收集信息
        up(index);
    }
}

//插花
void insert(int from,int flower){
    from++;
    //区间上还有多少个0
    int zeros = N-from+1- query(from,N,1,N,1);
    if(zeros==0){
        cout<<"Can not put any one."<<endl;
    }else{
        //能插多少朵花
        flower = min(flower,zeros);
        int start = find_zero(from,1);
        int end = find_zero(from,flower);
        cout<<start-1<<" "<<end-1<<endl;
        //执行插花操作
        update_value(start,end,1,N,1,1);
    }
}

//清除
void clear(int left,int right){
    left++;
    right++;
    int ans = query(left,right,1,N,1);
    cout<<ans<<endl;
    //清除花朵
    update_value(left,right,1,N,1,0);
    return;
}


//hdu-oj的一个插花问题
//操作1
//操作2清楚花瓶中的花朵，返回清理花朵的数量
//如何使用线段树求解
//线段树应该维护什么信息？
// 区间的最小值！区间之和！ 如何获取首个空瓶的位置呢？为什么是二分搜索？ 确实只需要维持累加和的信息，最小值是否是0可以直接根据累加和判断
//https://acm.hdu.edu.cn/showproblem.php?pid=4614
//问题是如何高效地获取首个空瓶的位置？这个如何考虑使用二分呢
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    //测试用例的数量
    cin>>T;
    int opt,x,y;
    while(T--!=0){
        cin>>N>>M;
        //建树
        build(1,N,1);
        for(int idx=0;idx<M;idx++){
            //有M个操作
            cin>>opt>>x>>y;
            if(opt==1){
                insert(x,y);
            }else if(opt==2){
                clear(x,y);
            }
        }
    }
    return 0;
}