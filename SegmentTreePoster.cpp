//
// Created by hsl on 2025/1/14.
//

#include<iostream>
//这里也有对数组进行排序的api
#include<algorithm>
using namespace std;

const int MAXN = 10000000+1;
const int MAXM = 1000+1;
//用于离散化的数组
int num[MAXM<<1];
int pl[MAXM];
int pr[MAXM];
int poster[MAXM<<3];
bool visited[MAXM];
int N,M;

/**
 * 用于去重离散化
 * @param size
 */
int collect(int size){
    //给数组排序
    sort(num+1,num+size+1);
    int cnt = 1;
    //去重
    for(int idx=2;idx<=size;idx++){
        if(num[cnt]!=num[idx]){
            num[++cnt] = num[idx];
        }
    }
    int last = cnt;
    for(int idx=2;idx<=last;idx++){
        if(num[idx-1]+1<num[idx]){
            num[++cnt] = num[idx-1]+1;
        }
    }
    //给数组排序
    sort(num+1,num+cnt+1);
    //返回总数量
    return cnt;
}

/*
 * 二分查找找排名
 */
int get_rank(int left,int right,int v){
    int ans = 0;
    int mid;
    while(left<=right){
        mid = (left+right)>>1;
        if(num[mid]>v){
            right = mid-1;
        }else if(num[mid]<v){
            left = mid+1;
        }else{
            ans = mid;
            break;
        }
    }
    return ans;
}

void build(int left,int right,int index){
    if(left<right){
        int mid = (left+right)>>1;
        build(left,mid,index<<1);
        build(mid+1,right,index<<1|1);
    }
    poster[index] = 0;
}

void down(int index){
    if(poster[index]!=0){
        poster[index<<1] = poster[index];
        poster[index<<1|1] = poster[index];
        poster[index] = 0;
    }
}

void update_value(int jobl,int jobr,int left,int right,int index,int value){
    if(jobl<=left&& jobr>=right){
        poster[index] = value;
    }else{
        //后来的会覆盖前面的
        down(index);
        int mid = (left+right)>>1;
        if(jobl<=mid){
            update_value(jobl,jobr,left,mid,index<<1,value);
        }
        if(jobr>mid){
            update_value(jobl,jobr,mid+1,right,index<<1|1,value);
        }
    }
}

/**
 * 本次查询全部海报沉底统计
 * @param jobl
 * @param jobr
 * @param left
 * @param right
 * @param index
 * @return
 */
int query(int jobl,int jobr,int left,int right,int index){
    if(left==right){
        if(poster[index]!=0 && visited[poster[index]]==false){
            visited[poster[index]] = true;
            return 1;
        }else{
            return 0;
        }
    }else{
        down(index);
        int mid = (left+right)>>1;
        int ans = 0;
        if(jobl<=mid){
            ans+= query(jobl,jobr,left,mid,index<<1);
        }
        if(jobr>mid){
            ans+= query(jobl,jobr,mid+1,right,index<<1|1);
        }
        return ans;
    }
}

//https://www.luogu.com.cn/problem/P3740
//线段树贴海报
//线段贴海报，先修改最后上浮沉底。后面的覆盖前面的
//问题是还需要离散化。真的恶心死了。这个离散化吗

// 1 2 3 4
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>N>>M;

    int size = 0;
    for(int idx=1;idx<=M;idx++){
        cin>>pl[idx];
        cin>>pr[idx];
        num[++size] = pl[idx];
        num[++size] = pr[idx];
    }

    size = collect(size);

    for(int idx=1;idx<=M;idx++){
        int jobl = get_rank(1,size,pl[idx]);
        //TODO 这里之前有个小问题，下标错误
        int jobr = get_rank(1,size,pr[idx]);
        //更新区间为编号idx的海报
        update_value(jobl,jobr,1,size,1,idx);
    }
    //获取答案
    int ans = query(1,size,1,size,1);
    cout<<ans<<endl;
    return 0;
}
