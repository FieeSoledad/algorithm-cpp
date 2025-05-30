//
// Created by hsl on 2024/11/5.
//

/**
 * 不同子序列II
 * 求一个字符串的不同子序列
 *
 */
#include<string>
#include<vector>
using namespace std;
class Solution{
public:
    int distinctSubSeqII(string s){
        int mod = 1000000007;
        vector<int> dp(26,0);
        int all = 1,newAdd;
        for(int idx=0;idx<s.length();idx++){
            //新增的
            newAdd = all-dp[(s.at(idx)-'a')];
            all+=newAdd;
            dp[s.at(idx)-'a']+=newAdd;
        }
        return (all-1)%mod;
    }
};