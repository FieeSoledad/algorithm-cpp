//
// Created by hsl on 2025/5/30.
//


#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 100000+1;
int n,m,root,mod;

int arr[MAXN];
int head[MAXN];
int next_edge[MAXN << 1];//链式前向星建图
int to[MAXN<<1];
int cntg = 0;

int fa[MAXN];
int dep[MAXN];
int siz[MAXN];
int son[MAXN];
int top[MAXN];
int dfn[MAXN];
int seg[MAXN];//记录dfn序对应的节点
int cntd;//dfn序号的计数

ll sum[MAXN<<2];//维护区间和
ll add_tag[MAXN<<2];


/**
 * 第一次递归统计fa，dep，siz，son
 *
 * @param u
 * @param f
 */
void dfs1(int u,int f){
    fa[u] = f;
    dep[u] = dep[f]+1;
    siz[u] = 1;
    for(int e = head[u],v;e!=0;e=next_edge[e]){
        v = to[e];
        if(v!=f){//递归统计
            dfs1(v,u);
        }
    }
    for(int e=head[u],v;e!=0;e=next_edge[e]){
        v = to[e];
        if(v!=f){
            siz[u]+=siz[v];
            if(son[u]==0 || siz[son[u]]<siz[v]){//设置子树规模最大的重儿子
                son[u] = v;
            }
        }
    }
}

/**
 * 来到节点u，节点u所在重链的头节点是t
 * @param u
 * @param t
 */
void dfs2(int u,int t){
    top[u] = t;//这里的t是什么呢，这个t是原始的编号
    dfn[u] = ++cntd;
    seg[cntd] = u;
    if(son[u]==0){//没有重儿子，叶子节点
        return;
    }
    //先遍历重儿子
    dfs2(son[u],u);
    for(int e=head[u],v;e!=0;e=next_edge[e]){
        v = to[e];
        if(v!=fa[u] && v!=son[u]){//不是父亲也不是重儿子
            dfs2(v,v);
        }
    }
}

void lazy(int i,ll v,int cnt){
    sum[i] = (sum[i]+v*cnt)%mod;
    //TODO
    add_tag[i] = (add_tag[i]+v)%mod;
}

/**
 * 线段树汇总信息
 * @param i
 */
void up(int i){
    sum[i] = (sum[i<<1]+sum[i<<1|1])%mod;
}


/**
 * 线段树懒信息下推
 * @param i
 * @param ln
 * @param rn
 */
void down(int i,int ln,int rn){
    if(add_tag[i]!=0){
        lazy(i<<1,add_tag[i],ln);
        lazy(i<<1|1,add_tag[i],rn);
        //TODO 懒信息清零
        add_tag[i] = 0;
    }
}

/**
 * 线段树的add
 * @param jobl
 * @param jobr
 * @param v
 * @param l
 * @param r
 * @param i
 */
void add(int jobl,int jobr,int v,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        lazy(i,v,r-l+1);
    }else{
        int mid = (l+r)>>1;
        down(i,mid-l+1,r-mid);
        if(jobl<=mid){
            add(jobl,jobr,v,l,mid,i<<1);
        }
        if(jobr>mid){
            add(jobl,jobr,v,mid+1,r,i<<1|1);
        }
        up(i);
    }
}

/**
 * 线段树查询区间和
 * @param jobl
 * @param jobr
 * @param l
 * @param r
 * @param i
 * @return
 */
ll query(int jobl,int jobr,int l,int r,int i){
    if(jobl<=l && jobr>=r){
        return sum[i];
    }else{
        int mid = (l+r)>>1;
        down(i,mid-l+1,r-mid);
        ll ans = 0;
        if(jobl<=mid){
            //TODO 记得及时取模
            ans = (ans+query(jobl,jobr,l,mid,i<<1))%mod;
        }
        if(jobr>mid){
            ans+= (ans+query(jobl,jobr,mid+1,r,i<<1|1))%mod;
        }
        return ans;
    }
}

/**
 * x到y这条路径上所有的节点增加v
 * @param x
 * @param y
 * @param v
 */
void path_add(int x,int y,int v){
    while(top[x]!=top[y]){
        if(dep[top[x]]<=dep[top[y]]){//y的深度更大
            add(dfn[top[y]],dfn[y],v,1,n,1);
            y = fa[top[y]];//切换重链
        }else{
            add(dfn[top[x]],dfn[x],v,1,n,1);
            x = fa[top[x]];
        }
    }
    //重链的头是一个的时候停止
    add(min(dfn[x],dfn[y]),max(dfn[x],dfn[y]),v,1,n,1);
}

/**
 * x到y的路径上的节点的权值和
 * @param x
 * @param y
 * @return
 */
ll path_sum(int x,int y){
    ll ans = 0;
    while(top[x]!=top[y]){
        if(dep[top[x]]<=dep[top[y]]){
            ans = (ans+query(dfn[top[y]],dfn[y],1,n,1))%mod;
            y = fa[top[y]];
        }else{
            ans = (ans+ query(dfn[top[x]],dfn[x],1,n,1))%mod;
            x = fa[top[x]];
        }
    }
    ans = (ans+query(min(dfn[x],dfn[y]),max(dfn[x],dfn[y]),1,n,1))%mod;
    return ans;
}

/**
 * 整棵子树上的节点值增加
 * @param x
 * @param y
 * @param v
 */
void subtree_add(int x,int v){
    add(dfn[x],dfn[x]+siz[x]-1,v,1,n,1);
}

ll subtree_sum(int x){
    return query(dfn[x],dfn[x]+siz[x]-1,1,n,1);
}

void add_edge(int u,int v){
    next_edge[++cntg] = head[u];
    head[u] = cntg;
    to[cntg] = v;
}


/**
 * 构建线段树
 * l和r都是dfn序号
 * @param l
 * @param r
 * @param i
 */
void build(int l,int r,int i){
    if(l==r){
        sum[l] = arr[seg[l]]%mod;
    }else{
        int mid = (l+r)>>1;
        build(l,mid,i<<1);
        build(mid+1,r,i<<1|1);
        up(i);
    }
}

//树链剖分的重链剖分
//https://www.luogu.com.cn/problem/P3384
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m>>root>>mod;

    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    for(int i=1;i<=n-1;i++){
        int x,y;
        cin>>x>>y;
        add_edge(x,y);
        add_edge(y,x);
    }
    //树链剖分
    dfs1(root,0);
    dfs2(root,root);
    build(1,n,1);
    for(int i=1;i<=m;i++){
        int x,y,z;
        int op;
        cin>>op;
        if(op==1){//路径+
            cin>>x>>y>>z;
            //TODO 确实都是原始的
            path_add(x,y,z);
        }else if(op==2){
            cin>>x>>y;
            cout<<path_sum(x,y)<<endl;
        }else if(op==3){//子树+
            cin>>x>>z;
            subtree_add(x,z);
        }else if(op==4){//子树查
            cin>>x;
            cout<<subtree_sum(x)<<endl;
        }
    }
    return 0;
}
