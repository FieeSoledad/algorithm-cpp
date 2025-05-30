//
// Created by hsl on 2025/1/17.
//
#include<iostream>
using namespace std;

//
const int MAXN = 5*10000+1;
const int MAXM = 5*10000+1;
//还是需要前缀和后缀诶。
int pre[MAXN<<2];
int suf[MAXN<<2];
int stack_[MAXM];
int top = 0;
int n,m;

void up(int lc,int rc,int i){
    int l = i<<1;
    int r = i<<1|1;
    pre[i] = pre[l]==lc ? (pre[l]+pre[r]):pre[l];
    suf[i] = suf[r]==rc ? (suf[r]+suf[l]):suf[r];
}

void build(int l,int r,int i){
    if(l==r){
        pre[i] = 1;
        suf[i] = 1;
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(mid-l+1,r-mid,i);
    }
}


void destory(int jobl,int jobr,int l,int r,int i){
    if(l==r){
        //没有懒
        pre[i] = 0;
        suf[i] = 0;
    }else{
        int mid = (l+r)>>1;
        if(jobr<=mid){
            destory(jobl,jobr,l,mid,i<<1);
        }
        if(jobl>mid){
            destory(jobl,jobr,mid+1,r,i<<1|1);
        }
        //汇总信息
        up(mid-l+1,r-mid,i);
    }
}

void repair(int jobl,int jobr,int l,int r,int i){
    if(l==r){
        pre[i] = 1;
        suf[i] = 1;
    }else{
        int mid = (l+r)>>1;
        if(jobr<=mid){
            repair(jobl,jobr,l,mid,i<<1);
        }
        if(jobl>mid){
            repair(jobl,jobr,mid+1,r,i<<1|1);
        }
        //汇总信息
        //TODO 错误1
        up(mid-l+1,r-mid,i);
    }
}

//最大最长的区间如何查询的，一定跟区间合并有关系吗？
//保证x在l-r之间
//单点x向左右扩展
int query(int x,int l,int r,int i){
    if(l==r){
        return pre[i];
    }else{
        int mid = (l+r)>>1;

        if(x<=mid){
            if(mid-x+1<=suf[i<<1]){
                return pre[i<<1|1]+suf[i<<1];
            }else{
                return query(x,l,mid,i<<1);
            }
        }else{
            if(x-mid<=pre[i<<1|1]){
                return suf[i<<1]+pre[i<<1|1];
            }else{
                return query(x,mid+1,r,i<<1|1);
            }
        }
    }
}
//https://www.luogu.com.cn/problem/P1503
//这类恢复还有可能多次恢复，所以不能使用last，要使用栈
//这题的查询挺有意思
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    build(1,n,1);
    char op;
    int x;
    for(int idx=0;idx<m;idx++){
        cin>>op;
        if(op=='D'){//摧毁
            cin>>x;
            destory(x,x,1,n,1);
            stack_[++top] = x;
        }else if(op=='R'){//修复上一个
            //使用变量last即可
            int last = stack_[top--];
            repair(last,last,1,n,1);
        }else if(op=='Q'){//每个士兵就是查询的地点
            cin>>x;
            int ans = query(x,1,n,1);
            cout<<ans<<endl;
        }
    }
    return 0;
}
