//
// Created by hsl on 2024/11/5.
//

/**
 * leetcode467最长环绕串中唯一的子串
 * 一个无限循环的base串，查找s中有多少不同非空子串也在base中出现
 */

/**
 * 思路：不同子串使用子串结尾字符的不同进行界定
 */
#include<string>
#include<vector>
#include<iostream>
using namespace std;
class Solution{
public:
    int findSubstringInWraproundString(string s){
        vector<int> dp(26,0);
        dp[(s.at(0)-'a')]=1;
        for(int idx=1,len=1;idx<s.length();idx++){
            int pre = s.at(idx-1)-'a';
            int cur = s.at(idx)-'a';
            if((pre==25&&cur==0) || ((pre+1)==cur)){
                len++;
            }else{
                len = 1;
            }
            dp[cur] = max(dp[cur],len);
        }
        int ans = 0;
        for(int idx=0;idx<dp.size();idx++){
            ans+=dp[idx];
        }
        return ans;
    }
};

int main(){
    Solution s;
    int ans = s.findSubstringInWraproundString("cac");
    cout<<"ans is "<<ans<<endl;
    return 0;
}
