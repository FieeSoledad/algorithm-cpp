//
// Created by hsl on 2025/1/14.
//

#include<iostream>
using namespace std;

const int MAXN = 100000+1;
const int MAXM = 100000+1;
int on_num[MAXN<<2];
bool rev[MAXN<<2];
int n,m;

void up(int index){
    on_num[index] = on_num[index<<1]+on_num[index<<1|1];
}

void down(int lc,int rc,int index){
    if(rev[index]){
        on_num[index<<1] = lc-on_num[index<<1];
        on_num[index<<1|1] = rc-on_num[index<<1|1];
        //TODO 懒信息的处理，不是直接设置成true就行了
        rev[index<<1] = !rev[index<<1];
        rev[index<<1|1] = !rev[index<<1|1];
        rev[index] = false;
    }
}

void build(int left,int right,int index){
    if(left==right){
        on_num[index] = 0;
        rev[index] = false;
    }else{
        int mid = (left+right)>>1;
        build(left,mid,index<<1);
        build(mid+1,right,index<<1|1);
        up(index);
    }
}

/**
 * 反转状态
 * @param jobl
 * @param jobr
 * @param left
 * @param right
 * @param index
 */
void rev_state(int jobl,int jobr,int left,int right,int index){
    //其实还是能懒的
    if(jobl<=left && jobr>=right){
        //懒信息是将操作的影响延迟下发
        on_num[index] = right-left+1-on_num[index];
        //懒信息
        rev[index] = !rev[index];
    }else{
        int mid = (left+right)>>1;
        down(mid-left+1,right-mid,index);
        if(jobl<=mid){
            rev_state(jobl,jobr,left,mid,index<<1);
        }
        if(jobr>mid){
            rev_state(jobl,jobr,mid+1,right,index<<1|1);
        }
        up(index);
    }
}

int query(int jobl,int jobr,int left,int right,int index){
    if(jobl<=left && jobr>=right){
        return on_num[index];
    }else{
        int mid = (left+right)>>1;
        down(mid-left+1,right-mid,index);
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


//https://www.luogu.com.cn/problem/P3870
//开关
//第一次提交直接TLE，只通过了两个测试用例，因为完全没有懒信息的更新诶。
//所以这个懒信息如何设计呢
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    build(1,n,1);
    int c,a,b;
    for(int idx=0;idx<m;idx++){
        cin>>c>>a>>b;
        if(c==0){//反转状态
            rev_state(a,b,1,n,1);
        }else if(c==1){//查询
            int ans = query(a,b,1,n,1);
            cout<<ans<<endl;
        }
    }
    return 0;
}
