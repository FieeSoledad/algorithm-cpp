//
// Created by hsl on 2024/12/17.
//

#include<iostream>
using namespace std;

const int MAXN=100000+1;
int n,m;
int num[MAXN];
int lnode[MAXN];
int rnode[MAXN];
int dist[MAXN];
int father[MAXN];


//算法初始化
void init(){
    dist[0] = -1;
    for(int idx=1;idx<=n;idx++){
        lnode[idx]=rnode[idx]=dist[idx]=0;
        father[idx] = idx;
    }
}

//并查集路径压缩的代码
int find_father(int i){
    father[i] = father[i]==i?i: find_father(father[i]);
    return father[i];
}

//两棵左偏树要进行合并
//返回合并后的头节点编号
int merge(int i,int j){
    if(i==0 || j==0){
        return i+j;
    }
    int temp;
    //保证i是头
    if(num[i]>num[j] || (num[i]==num[j] && i>j)){
        temp = i;
        i = j;
        j = temp;
    }
    rnode[i] = merge(rnode[i],j);
    //维护dist关系
    if(dist[lnode[i]]<dist[rnode[i]]){
        temp = lnode[i];
        lnode[i] = rnode[i];
        rnode[i] = temp;
    }
    //维护头节点dist
    dist[i] = dist[rnode[i]]+1;
    //维护查询时的father
    father[lnode[i]]=father[rnode[i]] = i;
    return i;
}

int pop(int i){
    //为什么多余？因为merge逻辑只会设置其中一个节点的father
    father[lnode[i]] = lnode[i];
    father[rnode[i]] = rnode[i];
    //合并
    father[i] = merge(lnode[i],rnode[i]);
    //清空有无必要？
    lnode[i] = rnode[i] = dist[i]=0;
    return father[i];
}
//可并堆左偏树
//https://www.luogu.com.cn/problem/P3377
//TODO我第一次的错误：删除节点father节点并没有置0
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    for(int idx=1;idx<=n;idx++){
        cin>>num[idx];
    }
    init();
    for(int idx=1;idx<=m;idx++){
        int opt;
        int x,y;
        cin>>opt;
        if(opt==1){//合并第x和第y个数字
            cin>>x>>y;
            if(num[x]!=-1 && num[y]!=-1){
                int h1 = find_father(x);
                int h2 = find_father(y);
                if(h1!=h2){//不在一个堆再合并
                    merge(h1,h2);
                }
            }
        }else{
            cin>>x;
            if(num[x]!=-1){
                int h = find_father(x);
                cout<<num[h]<<endl;
                pop(h);
                //删除后设置为0即可
                num[h] = -1;
            }else{
                cout<<-1<<endl;
            }
        }
    }
    return 0;
}