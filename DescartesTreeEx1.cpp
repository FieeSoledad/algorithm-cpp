//
// Created by hsl on 2024/12/2.
//

#include<iostream>
using namespace std;
typedef long long ll;

const int MAXN = 100000+1;
int n;
int head;
int diff_arr[MAXN];
int left_node[MAXN];
int right_node[MAXN];
int stack_[MAXN];
//中序遍历做收集
int collect[MAXN];
int ci = 0;

//之所以有这个返回值，是为了将指针连接操作放在父节点上完成
//如果头节点为空
int insert(int i,int idx){
    if(i==0){
        return idx;
    }
    if(diff_arr[idx] > diff_arr[i]){
        right_node[i] = insert(right_node[i],idx);
    }else{
        left_node[i] = insert(left_node[i],idx);
    }
    return i;
}

void build(){
    for(int i=1;i<=n;i++){
        head = insert(head,i);
    }
}


void preorder(int i){
    if(i==0){
        return;
    }
    collect[ci++] = i;
    preorder(left_node[i]);
    preorder(right_node[i]);

}

void build_descartes_tree(){
    for(int idx=1,top=0,pos=0;idx<=n;idx++){
        while(pos>0 && diff_arr[stack_[pos]] > diff_arr[idx]){
            pos--;
        }
        if(pos>0){
            right_node[stack_[pos]] = idx;
        }
        if(pos<top){
            left_node[idx] = stack_[pos+1];
        }
        stack_[++pos] = idx;
        top = pos;
    }
    //建树完成后，栈中一直维护着整棵树的右边界
    head = stack_[1];
}

//https://www.luogu.com.cn/problem/P1377
//笛卡尔树的应用
//依次插入，返回字典序尽量小的序列
//思路：我直接建起来先序遍历即可
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n;
    for(int idx=1;idx<=n;idx++){
        int num;
        cin>>num;
        //记录序号
        diff_arr[num] = idx;
    }
    //build();
    build_descartes_tree();
    preorder(head);
    for(int idx=0;idx<n;idx++){
        cout<<collect[idx]<<" ";
    }
    return 0;
}

