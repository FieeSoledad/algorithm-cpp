//
// Created by hsl19 on 2026/5/18.
//
#include<iostream>
#include<queue>
#include<algorithm>
#include<vector>
using namespace std;
typedef long long ll;

const int MAXN = 5000+1;
const int MAXM = 2*1e5+1;
int n,m;
int father[MAXN];// 这里默认为0


struct Info{
    int src;
    int des;
    int val;
};

struct cmp{
    bool operator()(Info& a, Info& b){
        return a.val - b.val;
    }
};

int find(int i){
    if(father[i] != i){
        father[i] = find(father[i]);
    }
    return father[i];
}

bool union_(int a, int b){
    // 合并吗？
    int fa = find(a);
    int fb = find(b);
    if(fa != fb){
        father[fa] = fb;
        return true;
    }else{
        return false;
    }
}



// 使用Kruskal算法实现最小生成树
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        father[i] = i;// 此时父节点就是自己
    }
    vector<Info> edges;
    for(int i=0;i<m;i++){
        Info edge;
        cin>>edge.src;
        cin>>edge.des;
        cin>>edge.val;
        edges.push_back(edge);
    }
    sort(edges.begin(), edges.end(), [](Info& a, Info& b){
        return a.val < b.val;
    });
    int ans = 0;
    int num = 0;
    for(auto& edge:edges){
        if(union_(edge.src, edge.des)){// 可以合并
            ans += edge.val;
            num += 1;
        }
    }
    if(num != n-1){
        cout<<"orz"<<endl;
    }else{
        cout<<ans<<endl;
    }

    return 0;
}
