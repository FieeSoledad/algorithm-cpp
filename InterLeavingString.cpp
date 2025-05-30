//
// Created by hsl on 2024/11/6.
//

/**
 * 交错字符串
 * 验证s能否由s1和s2两个字符串交错组成
 * 居然还做到了Bug Free
 * dp[i][j]的定义是s1的前i个字符和s2的前j个字符能否交错组成s的前i+j个字符
 */
#include<string>
#include<vector>
using namespace std;

class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if(s3.length()!=s1.length()+s2.length()){
            return false;
        }
        vector<vector<bool>> dp(s1.length()+1,vector<bool>(s2.length()+1,false));

        dp[0][0] = true;
        for(int i=1;i<=s2.length();i++){
            if(s2.at(i-1)==s3.at(i-1)){

                dp[0][i] = dp[0][i-1];
            }else{
                dp[0][i] = false;
            }
        }
        for(int i=1;i<=s1.length();i++){
            if(s1.at(i-1)==s3.at(i-1)){
                dp[i][0] = dp[i-1][0];
            }else{
                dp[i][0] = false;
            }
        }
        for(int i=1;i<=s1.length();i++){
            for(int j=1;j<=s2.length();j++){
                if(s1.at(i-1)!=s3.at(i+j-1) && s2.at(j-1)!=s3.at(i+j-1)){
                    dp[i][j] = false;
                }else if(s1.at(i-1)==s3.at(i+j-1) && s2.at(j-1)==s3.at(i+j-1)){
                    dp[i][j] = dp[i-1][j] || dp[i][j-1];
                }else if(s1.at(i-1)==s3.at(i+j-1) && s2.at(j-1)!=s3.at(i+j-1)){
                    dp[i][j] = dp[i-1][j];
                }else {
                    dp[i][j] = dp[i][j-1];
                }
            }
        }
        return dp[s1.length()][s2.length()];
    }
};