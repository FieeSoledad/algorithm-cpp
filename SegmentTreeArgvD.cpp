//
// Created by hsl on 2025/1/15.
//
#include<iostream>
using namespace std;

const int MAXN = 100000+1;
int N,M;
double range_sum[MAXN<<2];
double range_2sum[MAXN<<2];
double arr[MAXN];
double add_step[MAXN<<2];

void up(int i){
    range_sum[i] = range_sum[i<<1]+range_sum[i<<1|1];
    range_2sum[i] = range_2sum[i<<1]+range_2sum[i<<1|1];
}

//卡住我的是方差的汇总
void build(int l,int r,int i){
    if(l==r){
        range_sum[i] = arr[l];
        //TODO ERROR1
        range_2sum[i] = arr[l]*arr[l];
    }else{
        //TODO ERROR2
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
    }
}

void lazy_inc(int cnt,int i,double v){
    range_2sum[i]+=cnt*v*v+2*v*range_sum[i];
    range_sum[i]+=v*cnt;
    //TODO ERROR7 +=
    add_step[i] += v;
}

void down(int lc,int rc,int i){
    if(add_step[i]!=0){
        lazy_inc(lc,i<<1,add_step[i]);
        //TODO ERROR3
        lazy_inc(rc,i<<1|1,add_step[i]);
        add_step[i]=0;
    }
}

void inc(int jobl,int jobr,int l,int r,int i,double v){
    if(jobl<=l && jobr>=r){
        lazy_inc(r-l+1,i,v);
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        if(jobl<=mid){
            inc(jobl,jobr,l,mid,i<<1,v);
        }
        if(jobr>mid){
            inc(jobl,jobr,mid+1,r,i<<1|1,v);
        }
        up(i);
    }
}

double query_sum(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return range_sum[i];
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        double ans = 0;
        if(jobl<=mid){
            ans+= query_sum(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans+= query_sum(jobl,jobr,mid+1,r,i<<1|1);
        }
        return ans;
    }
}


double query_avg(int jobl,int jobr){
    double sum = query_sum(jobl,jobr,1,N,1);
    int cnt = jobr-jobl+1;
    double ans = sum/cnt;
    return ans;
}

double query_2sum(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return range_2sum[i];
    }else{
        int mid = (l+r)>>1;
        down(mid-l+1,r-mid,i);
        double ans = 0;
        if(jobl<=mid){
            ans+= query_2sum(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans+= query_2sum(jobl,jobr,mid+1,r,i<<1|1);
        }
        return ans;
    }
}


double query_s2(int jobl,int jobr){
    int cnt = jobr-jobl+1;
    double sum2 = query_2sum(jobl,jobr,1,N,1);
    double avg = query_avg(jobl,jobr);
    //TODO ERROR5 加减号
    double ans = (sum2+cnt*avg*avg-2*avg* query_sum(jobl,jobr,1,N,1))/cnt;
    return ans;
}

//https://www.luogu.com.cn/problem/P1471
//平方数和方差
//线段树维护平方和和累加和即可
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>N>>M;
    for(int idx=1;idx<=N;idx++){
        cin>>arr[idx];
    }
    //TODO ERROR4
    build(1,N,1);
    int op,x,y;
    double k;
    for(int idx=0;idx<M;idx++){
        cin>>op;
        if(op==1){
            cin>>x>>y>>k;
            inc(x,y,1,N,1,k);
        }else if(op==2){
            cin>>x>>y;
            double ans = query_avg(x,y);
            //ERROR6 保留4位数小数
            printf("%.4f\n",ans);
        }else if(op==3){
            cin>>x>>y;
            double ans = query_s2(x,y);
            printf("%.4f\n",ans);
        }
    }
    return 0;
}
