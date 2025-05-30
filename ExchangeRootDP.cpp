//
// Created by hsl on 2024/11/15.
//

//换根DP
#include<iostream>
#include<cstring>
using namespace std;
typedef long long ll;

//链式前向星建图

const int MAXN = 1000000+1;
int head[MAXN];
int nextEdge[MAXN << 1];
int to[MAXN<<1];
int cnt = 1;
ll sum[MAXN];
int size[MAXN];
ll dp[MAXN];
int n;

//初始化内存
void build(){
    memset(head,0,sizeof(head));
    memset(nextEdge, 0, sizeof(nextEdge));
    memset(to,0,sizeof(to));
    memset(sum,0,sizeof(sum));
    memset(size,0,sizeof(size));
    memset(dp,0,sizeof(dp));
    cnt = 1;
}

//链式前向星中加边
void addEdge(int u,int v){
    nextEdge[cnt] = head[u];
    to[cnt] = v;
    head[u]=cnt;
    cnt++;
}

//构造出sum和size信息
void dfs1(int cur,int father) {
    for (int e = head[cur], v; e != 0; e = nextEdge[e]) {
        v = to[e];
        if (v != father) {
            dfs1(v, cur);
        }
    }
    size[cur] = 1;
    sum[cur] = 0;
    for(int e = head[cur],v;e != 0;e = nextEdge[e]){
        v = to[e];
        if(v!=father){
            size[cur]+=size[v];
            sum[cur]+=size[v]+sum[v];
        }
    }
}

void dfs2(int cur,int father){
    for(int e = head[cur],v;e!=0;e = nextEdge[e]){
        v = to[e];
        if(v!=father){
            dp[v] = dp[cur]-size[v]+n-size[v];
            dfs2(v,cur);
        }
    }
}

int main(){
    build();
    cin>>n;
    for(int i=1,u,v;i<n;i++){
        cin>>u;
        cin>>v;
        addEdge(u,v);
        addEdge(v,u);
    }
    dfs1(1,0);

    //距离和
    dp[1] = sum[1];
    dfs2(1,0);

    ll ans = INT_MIN;
    int index = 0;
    for(int i=1;i<=n;i++){
        if(dp[i]>ans){
            ans = dp[i];
            index = i;
        }
    }
    cout<<index<<endl;
    return 0;
}