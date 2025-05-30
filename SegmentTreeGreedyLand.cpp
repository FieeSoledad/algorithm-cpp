//
// Created by hsl on 2025/1/14.
//
#include<iostream>
using namespace std;

const int MAXN = 100000+1;
const int MAXM = 1000+1;
//防线长度
int n;
//总的操作次数
int m;
//用于维护开头和结尾的数量
int s_num[MAXN<<2];
int e_num[MAXN<<2];

void up(int i){
    s_num[i] = s_num[i<<1]+s_num[i<<1|1];
    e_num[i] = e_num[i<<1]+e_num[i<<1|1];
}

void build(int l,int r,int i){
    if(l==r){
        s_num[i] = 0;
        e_num[i] = 0;
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
    }
}

void range_add_s(int pos,int l,int r,int i){
    if(l==r && l==pos){
        s_num[i]+=1;
    }else{
        int mid = (l+r)>>1;
        if(pos<=mid){
            range_add_s(pos,l,mid,i<<1);
        }
        if(pos>mid){
            range_add_s(pos,mid+1,r,i<<1|1);
        }
        up(i);
    }
}

void range_add_e(int pos,int l,int r,int i){
    if(l==r && l==pos){
        //TODO 这里的index
        e_num[i]++;
    }else{
        int mid = (l+r)>>1;
        if(pos<=mid){
            range_add_e(pos,l,mid,i<<1);
        }
        if(pos>mid){
            range_add_e(pos,mid+1,r,i<<1|1);
        }
        up(i);
    }
}

int query_s(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return s_num[i];
    }else{
        int mid = (l+r)>>1;
        int ans = 0;
        if(jobl<=mid){
            ans += query_s(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans += query_s(jobl,jobr,mid+1,r,i<<1|1);
        }
        return ans;
    }
}

int query_e(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return e_num[i];
    }else{
        int mid = (l+r)>>1;
        int ans = 0;
        if(jobl<=mid){
            ans+=query_e(jobl,jobr,l,mid,i<<1);
        }
        if(jobr>mid){
            ans+= query_e(jobl,jobr,mid+1,r,i<<1|1);
        }
        return ans;
    }
}

//https://www.luogu.com.cn/problem/P2184
//贪婪大陆，这是一个布放地雷的故事
//多少种不同的地雷
//看清楚题目，还有就是我不太会这种东西
//本题应该说大有来头
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m;
    build(1,n,1);
    int q,l,r;
    for(int idx=0;idx<m;idx++){
        cin>>q>>l>>r;
        if(q==1){//布雷
            range_add_s(l,1,n,1);
            range_add_e(r,1,n,1);
        }else if(q==2){//询问
            int s = query_s(1,r,1,n,1);
            int e = query_e(1,l-1,1,n,1);
            int ans = s-e;
            cout<<ans<<endl;
        }
    }
    return 0;
}
