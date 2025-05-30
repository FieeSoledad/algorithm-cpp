//
// Created by hsl on 2024/9/16.
//

/**
 * https://www.luogu.com.cn/problem/P1253
 * 洛谷线段树
 */
#include<iostream>
using namespace std;

const int MAXN = 1000001;
//这是线段树维护的信息
long array[MAXN<<2];
//维护区间最大值
long segment_max[MAXN<<2];
//维护区间增加的懒信息
long add[MAXN<<2];
//维护区间修改的懒信息
long change[MAXN<<2];
//维护区间是否更新的懒信息
//表示是否有更新信息，因为change更新的内容有可能是0，必须这样使用
bool update[MAXN<<2];

void lazy_change(int index,int value){
    segment_max[index] = value;
    change[index] = value;
    add[index] = 0;
    update[index] = true;
}

void lazy_add(int index,int value){
    //有更新信息
    if(update[index]){
        segment_max[index]+=value;
        change[index]+=value;
    }else {
        segment_max[index]+=value;
        add[index] += value;
    }
}
/**
 * 懒信息下推
 */
void down(int index){
    if(update[index]){
        lazy_change(index<<1,change[index]);
        lazy_change(index<<1|1,change[index]);
        update[index] = false;
    }
    if(add[index]!=0){
        lazy_add(index<<1,add[index]);
        lazy_add(index<<1|1,add[index]);
        add[index] = 0;
    }
}

void up(int index){
    segment_max[index] = max(segment_max[index<<1],segment_max[index<<1|1]);
}
/**
 * 整个区间的信息在index
 * @param left
 * @param right
 * @param index
 */
void buildTree(int left,int right,int index){
    if(left==right){
        segment_max[index] = array[left];
    }else{
        int mid = (left+right)>>1;
        buildTree(left,mid,index<<1);
        buildTree(mid+1,right,index<<1|1);
        up(index);
    }
    add[index] = 0;
    change[index] = 0;
    update[index] = false;
}

void range_change(int jobL,int jobR,int left,int right,int index,int value){
    //全覆盖则懒更新
    if(jobL<=left&&jobR>=right){
        lazy_change(index,value);
    }else{
        down(index);
        int mid = (left+right)>>1;
        if(jobL<=mid){
            range_change(jobL,jobR,left,mid,index<<1,value);
        }
        if(jobR>mid){
            range_change(jobL,jobR,mid+1,right,index<<1|1,value);
        }
        up(index);
    }
}

/**
 * 设置为多少
 * 增加多少
 * 最大是多少
 * 这里逻辑要自洽，change是可以覆盖add的
 * @param index
 * @param value
 */
void range_add(int jobL,int jobR,int left,int right,int index,int value){
    if(jobL<=left && jobR>=right){
        lazy_add(index,value);
    }else{
        //下推懒信息
        down(index);
        int mid = (left+right)>>1;
        if(jobL<=mid){
            range_add(jobL,jobR,left,mid,index<<1,value);
        }
        if(jobR>mid){
            range_add(jobL,jobR,mid+1,right,index<<1|1,value);
        }
        up(index);
    }
}

/**
 * 查询区间最大值
 * @param jobL
 * @param jobR
 * @param left
 * @param right
 * @param index
 * @return
 */
long query(int jobL,int jobR,int left,int right,int index){

    if(jobL<=left && jobR>=right){
        return segment_max[index];
    }else{
        //懒信息下推
        down(index);
        long ans = 0;
        int mid = (left+right)>>1;
        if(jobL<=mid){
            ans = max(ans, query(jobL,jobR,left,mid,index<<1));
        }
        if(jobR>mid){
            ans = max(ans, query(jobL,jobR,mid+1,right,index<<1|1));
        }
        return ans;
    }
}
int main(){
    int n,q;

    cin>>n>>q;
    for(int idx=1;idx<=n;idx++){
        cin>>array[idx];
    }

    buildTree(1,n,1);
    for(int idx=1,op,jobL,jobR,value;idx<=q;idx++){
        cin>>op;
        if(op==1){//修改为
            cin>>jobL;
            cin>>jobR;
            cin>>value;
            range_change(jobL,jobR,1,n,1,value);
        }else if(op==2){//增加
            cin>>jobL;
            cin>>jobR;
            cin>>value;
            range_add(jobL,jobR,1,n,1,value);
        }else if(op==3){//查询最大值
            cin>>jobL;
            cin>>jobR;
            cout<<query(jobL,jobR,1,n,1)<<endl;
        }
    }
}


