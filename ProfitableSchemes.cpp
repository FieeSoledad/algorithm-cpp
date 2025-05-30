//
// Created by hsl on 2024/11/7.
//

/**
* 盈利计划
*/

/**
 * 思路：很明显的动态规划
 * 之所以可以直接使用二维动态规划解决，是因为更新当前状态时已经暗含了在前面状态基础上子问题的结果
 *
 *
 * 这个比较奇妙，这个是员工不能多，利润不能少，所以dp的定义不能太过严格
 *
 *
 */
#include<vector>
#include<iostream>
using namespace std;
const int mod = 1000000007;
class Solution{
public:
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        //都只有0种方案
        vector<vector<long>> dp(minProfit+1,vector<long>(n+1,0));
        //初始化
        //状态转移依赖左上角的值
        dp[0][0] = 1;
        for(int idx=1;idx<=minProfit;idx++){
            dp[idx][0] = 0;
        }
        for(int idx=1;idx<=n;idx++){
            dp[0][idx] = 1;
        }
        for(int idx=0;idx<profit.size();idx++){
            //以是否完成当前工作作为尝试策略，方案数量
            //需要考虑哪些格子会受到影响,其实都会，要求的盈利不少于，不是严格等于
            for(int p=minProfit;p>=0;p--){
                for(int workers=n;workers>=group[idx];workers--){
                    //在当前子集上要使用workers个工人盈利p，有多少种方案数，应该是不断地增加上去吧，是要刚好使用呢
                    //1.完成当前的工作
                    //2.不完成当前的工作
                    if(workers>=group[idx]){
                        dp[p][workers] += dp[p-profit[idx]<=0?0:p-profit[idx]][workers-group[idx]];
                        dp[p][workers] %= mod;
                    }
                }
            }
        }
        for(int idx=0;idx<= minProfit;idx++){
            for(int j=0;j<=n;j++){
                cout<<dp[idx][j];
            }
            cout<<endl;
        }
        return dp[minProfit][n]%mod;
    }
};

int main(){
    Solution s;
    vector<int> g = {2,3,5};
    vector<int> p = {6,7,8};
    cout<<s.profitableSchemes(10,5,g,p)<<endl;
}