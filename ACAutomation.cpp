//
// Created by hsl on 2024/12/11.
//
#include<iostream>
#include<string>
#include<vector>
using namespace std;
//目标串
//TODO预处理数组可能的问题
const int MAXN = 200000+1;
//总字符数量不超过
const int MAXS = 200000+1;
//目标串节点结尾的编号
int endArr[MAXN];
int cnt=0;
int tree[MAXS][26];
int fail[MAXS];
//记录词频
int times[MAXS];
int box[MAXS];

//用于链式前向星（词频统计的优化）
int edge=0;
int head[MAXS];
int next_edge[MAXS];
int to[MAXS];
//链式前向星建图然后
void add_edge(int u,int v){
    edge++;
    next_edge[edge] = head[u];
    to[edge]=v;
    head[u] = edge;
}

//经典前缀树的构建代码
void insert(int i,string s){
    int len = s.size();
    int node = 0;
    for(int j=0;j<len;j++){
        int cur = s[j]-'a';
        if(tree[node][cur]==0){
            tree[node][cur]=++cnt;
        }
        node=tree[node][cur];
    }
    endArr[i] = node;
}

//设置fail指针
//我的疑问：既然已经有了直通表，为什么还要保留fail指针嗯
//AC自动机建立的过程是什么啊woc
void setFail(){
    int l = 0;
    int r = 0;
    for(int idx=0;idx<26;idx++){
        if(tree[0][idx]>0){
            box[r++] = tree[0][idx];
        }
    }
    while(l<r){
        int cur = box[l++];
        for(int idx=0;idx<=25;idx++){
            if(tree[cur][idx]==0){//没有路，设置直通表
                tree[cur][idx] = tree[fail[cur]][idx];
            }else{//父亲节点设置子节点的fail指针的
                //这里之所以能直接设置，是因为父节点的fail指针已经设置了，所以tree不会是0。但是如何保证一定有通路的呢？
                fail[tree[cur][idx]] = tree[fail[cur]][idx];
                box[r++] = tree[cur][idx];
            }
        }
    }
}

//词频统计
void dfs(int cur){
    for(int e = head[cur];e!=0;e=next_edge[e]){
        dfs(to[e]);
        times[cur]+=times[to[e]];
    }
}

//Ac自动机实现
//https://ww.luogu.com.cn/problem/P5357
//理解fail指针如何保存最大的可能和fail指针建立的优化
//父节点设置子节点的fail指针的含义：要强行保留这个孩子节点作为另外一个目标串的前缀
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin>>n;
    vector<string> strs(n,"");
    for(int idx=0;idx<strs.size();idx++){
        cin>>strs[idx];
    }
    string S;
    cin>>S;
    for(int idx=1;idx<=strs.size();idx++){
        insert(idx,strs[idx-1]);
    }
    setFail();
    for(int idx=0,cur=0;idx<S.size();idx++){
        cur = tree[cur][S[idx]-'a'];
        times[cur]++;
    }
    //根据fail指针建反图
    for(int i=1;i<=cnt;i++){
        add_edge(fail[i],i);
    }

    dfs(0);
    for(int i=1;i<=n;i++){
        cout<<times[endArr[i]]<<endl;
    }
    return 0;
}
