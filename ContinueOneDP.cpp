//
// Created by hsl on 2025/1/15.
//
#include<iostream>
#include<vector>
using namespace std;

//对的呀，有一个变量是机会呀。我真的是无语了。
int solution(vector<int>& arr,int k){
    //容器的大小
    int len = arr.size();
    //dp[i][j],代表在对于数组前i个串的前提下，给了j次机会的最长0串，这里已经全部初始化为0
    vector<vector<int>> dp(len+1, vector<int>(k+1,0));

    int ans = 0;
    //fill dp arr
    //the order is determined by principle
    for(int i=1;i<=len;i++){
        for(int j = 0;j<=k;j++){
            if(arr[i-1]==1){
                dp[i][j] = dp[i-1][j]+1;
                ans = max(ans,dp[i][j]);
                //TODO 第一次我漏掉了更新dp[i][0]，因为某些地方所谓的其实可以很长，不为1
            }else if(arr[i-1]==0){
                if(j==0){
                    dp[i][j] = 0;
                }else{
                    dp[i][j] = dp[i-1][j-1]+1;
                }
                ans = max(ans,dp[i][j]);
            }
        }
    }

    return ans;
}


//一直想解决的普联二面的一道题
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> arr = {0,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1};
    cout<<solution(arr,1)<<endl;

    return 0;
}
