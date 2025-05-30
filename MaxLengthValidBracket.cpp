//
// Created by hsl on 2024/11/5.
//

#include<string>
#include<vector>
using namespace std;
//动态规划，经典最长有效括号
class Solution{
public:
    int longestValidParentheses(string s){
        if(s==""){
            return 0;
        }
        //含义：以i为结尾的最长有效括号
        int ans = 0;
        vector<int> dp(s.length(),0);
        for(int idx=1;idx<s.length();idx++){
            if(s.at(idx)==')'){
                //...()
                if(s.at(idx-1)=='('){
                    dp[idx] = 2+(idx-2>=0?dp[idx-2]:0);
                //...))
                }else if(idx-1-dp[idx-1]>=0 && s.at(idx-1-dp[idx-1])=='('){
                    //        当前括号配对+内部包含+前面连续
                    dp[idx] = 2+dp[idx-1]+(idx-1-dp[idx-1]-1>=0?dp[idx-1-dp[idx-1]-1]:0);
                }
                ans = max(ans,dp[idx]);
            }
        }
        return ans;
    }
};