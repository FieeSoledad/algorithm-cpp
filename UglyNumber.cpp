//
// Created by hsl on 2024/11/5.
//
#include<vector>
using namespace std;
class Solution{
public:
    int nthUglyNumber(int n) {
        vector<int> dp(n+1);
        dp[1] = 1;
        for(int i=2,i2=1,i3=1,i5=1;i<=n;i++){
            int a = dp[i2]*2;
            int b = dp[i3]*3;
            int c = dp[i5]*5;
            int min = std::min(a,std::min(b,c));
            if(a==min){
                i2++;
            }
            if(b==min){
                i3++;
            }
            if(c==min){
                i5++;
            }
            dp[i] = min;
        }
        return dp[n];
    }
};