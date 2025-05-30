//
// Created by hsl on 2024/11/15.
//

//为树形大陆选择首都
//树是有向树
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
//转移方程是简单的
//每条边的方向要如何确定呢

const int MAXN = 2*100000+1;
int n;

int head[MAXN];
int nextEdge[MAXN<<1];
int to[MAXN<<1];
//权重可以用于记录方向
int weight[MAXN<<1];
int cnt = 1;
int dp[MAXN];



void addEdge(int u,int v,int w){
    nextEdge[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

int ansFrom1 = 0;
void dfs1(int cur,int father){
    for(int e=head[cur],v;e!=0;e=nextEdge[e]){
        v=to[e];
        if(v!=father){
            ansFrom1+=weight[e];
            dfs1(v,cur);
        }
    }
}

void dfs2(int cur,int father){
    for(int e=head[cur],v;e!=0;e=nextEdge[e]){
        v = to[e];
        if(v!=father){
            if(weight[e]==1){
                dp[v] = dp[cur]-1;
            }else{
                dp[v] = dp[cur]+1;
            }
            dfs2(v,cur);
        }
    }
}

void build(){
    cnt = 1;
}

int main(){
    build();
    cin>>n;
    for(int i=1,u,v;i<n;i++) {
        cin >> u;
        cin >> v;
        //使用权重记录方向
        addEdge(u, v,0);
        addEdge(v, u,1);
    }
    dfs1(1,0);
    dp[1] = ansFrom1;
    dfs2(1,0);

    int min = n+1;
    for(int i=1;i<=n;i++){
        if(dp[i]<min){
            min = dp[i];
        }
    }
    vector<int> ans;
    for(int i=1;i<=n;i++){
        if(dp[i]==min){
            ans.push_back(i);
        }
    }
    std::sort(ans.begin(),ans.end());
    cout<<min<<endl;
    for(auto& item:ans){
        cout<<item<<" ";
    }
    return 0;
}
