//
// Created by hsl on 2024/11/15.
//


//树的染色
//使用换根dp解决
#include<iostream>
using namespace std;
typedef long long ll;

int n;
const int MAX = 1000000+1;
int head[MAX];
int nextEdge[MAX<<1];
int to[MAX<<1];
int cnt;
int subtree_size[MAX];
ll dp[MAX];

void build(){
    cnt = 1;
}

void addEdge(int u,int v){
    nextEdge[cnt] = head[u];
    to[cnt] = v;
    head[u] = cnt++;
}

void dfs1(int cur,int father){
    for(int e = head[cur],v;e!=0;e=nextEdge[e]){
        v = to[e];
        if(v!=father){
            dfs1(v,cur);
        }
    }
    subtree_size[cur] = 1;
    for(int e=head[cur],v;e!=0;e=nextEdge[e]) {
        v = to[e];
        if (v != father) {
            subtree_size[cur] += subtree_size[v];
        }
    }
}

ll computeDpOne(int cur,int father){
    ll ans = subtree_size[cur];
    for(int e=head[cur],v;e!=0;e=nextEdge[e]){
        v = to[e];
        if(v!=father){
            ans+= computeDpOne(v,cur);
        }
    }
    return ans;
}

void dfs2(int cur,int father){
    for(int e=head[cur],v;e!=0;e=nextEdge[e]){
        v = to[e];
        if(v!=father){
            //这个递推公式有问题吗？
            dp[v] = dp[cur]+n-subtree_size[v]-subtree_size[v];
            dfs2(v,cur);
        }
    }
}

int main(){
    build();
    cin>>n;
    for(int idx=1,u,v;idx<n;idx++){
        cin >>u;
        cin >>v;
        addEdge(u,v);
        addEdge(v,u);
    }
    dfs1(1,0);
    //计算dp[1]
    dp[1]= computeDpOne(1,0);
    dfs2(1,0);
    ll ans = -1;
    for(int i=1;i<=n;i++){
        if(dp[i]>ans){
            ans =  dp[i];
        }
    }
    cout<<ans<<endl;
    return 0;
}
