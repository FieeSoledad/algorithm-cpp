//
// Created by hsl on 2024/12/3.
//
#include<iostream>
#include<cstring>
using namespace std;
typedef long long ll;

const int mod = 1e9+7;
const int MAXN = 2*100000+1;
int cases;
int m,n;
//构建笛卡尔树
int arr_a[MAXN];
//这里就是维护了笛卡尔树的指针
int left_node[MAXN];
int right_node[MAXN];
//构建笛卡尔树的辅助栈
int stack_[MAXN];
//用于存储
int temp[MAXN];

void build_descartes_tree(){
    for(int idx=1,top=0,pos=0;idx<=n;idx++) {
        while (pos > 0 && arr_a[stack_[pos]] < arr_a[idx]) {
            pos--;
        }
        if (pos > 0) {
            right_node[stack_[pos]] = idx;
        }
        if (pos < top) {
            left_node[idx] = stack_[pos + 1];
        }
        stack_[++pos] = idx;
        top = pos;
    }
}

//cpp如何保证数组的引用传递呢
void dfs(int head,int** dp){
    if(head==0){
        return;
    }
    dfs(left_node[head],dp);
    dfs(right_node[head],dp);
    for(int idx=1;idx<=m;idx++){
        //这里强转为了避免计算时溢出
        temp[idx] =  (ll)dp[left_node[head]][idx-1]*dp[right_node[head]][idx]%mod;
    }
    for(int idx=1;idx<=m;idx++){
        dp[head][idx] = (dp[head][idx-1]+temp[idx])%mod;
    }
}

void clear(){
    memset(left_node,0,sizeof (left_node));
    memset(right_node,0,sizeof(right_node));
}

//https://codeforces.com/problemset/problem/1748/E
//简而言之就是一个数组的区间最左端最大值的分布相同的数量。限制1~m
//div2的E题，大概在2000分-2400分
//笛卡尔树的应用，最左端最大值的划分
//树型DP+DescartesTree
//TimeLimits 2seconds
//MemoryLimits 512megabytes (512MB)  //静态空间准备确实会超限。 n*m<10**6
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>cases;
    while(cases>0) {
        cin >> n >> m;
        for (int idx = 1; idx <= n; idx++) {
            cin >> arr_a[idx];
        }
        build_descartes_tree();
        //dp[i][j]表示的是节点编号<=j的构树数量
        int** dp = new int*[n + 1];
        for (int idx = 0; idx <= n; idx++) {
            dp[idx] = new int[m + 1];
        }
        for (int idx = 0; idx <= m; idx++) {
            dp[0][idx] = 1;
        }
        //初始化，这里分配的内存可能是垃圾值
        for (int idx = 1; idx <= n; idx++) {
            dp[idx][0] = 0;
        }
        //构建笛卡尔树后栈底就是整颗树的头节点
        dfs(stack_[1], dp);
        cout << dp[stack_[1]][m]<<endl;
        clear();
        cases--;
    }
    return 0;
}