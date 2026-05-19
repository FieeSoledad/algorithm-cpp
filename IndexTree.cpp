//
// Created by hsl19 on 2026/5/19.
//

#include<iostream>
using namespace std;
typedef long long ll;


const int MAXN = 5e5 + 1;
int tree[MAXN];
int n,m;

int lowbit(int n){
    return n & (-n);
}

/**
 * 给原始数组index处+1
 * @param index
 * @param val
 */
void add(int index, int val){
    while(index <= n){
        tree[index] += val;
        index += lowbit(index);
    }
}

/**
 * 查询1-index的累加和
 * @param index
 * @return
 */
int query(int index){
    int ans = 0;
    while (index>0){
        ans += tree[index];
        index -= lowbit(index);
    }
    return ans;
}

int range_query(int left, int right){
    return query(right)- query(left-1);
}


// https://www.luogu.com.cn/problem/P3374
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>m;
    int val;
    for(int i=1;i<=n;i++){
        cin>>val;
        add(i, val);
    }
    int op, index;
    for(int i=0;i<m;i++){
        cin>>op>>index>>val;
        if(op==1){
            add(index, val);
        }else if(op==2){
            cout<<range_query(index,val)<<endl;
        }
    }
    return 0;
}