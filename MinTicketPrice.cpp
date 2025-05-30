//
// Created by hsl on 2024/10/25.
//

//通行证问题，确实很有意思
#include<vector>
#include<array>
#include<iostream>
using namespace std;

//使用列表初始化
//很奇怪，怎么iostream怎么默认包含这么多东西
//时间复杂度应该是O(n)*枚举代价，枚举代价虽然是两个循环，但是应该是一个常熟时间复杂度
const vector<int> durations = {1,7,30};
class Solution{
public:
    int mincostTickets(vector<int>& days,vector<int>& costs){

        //严格位置依赖的动态规划
        //cpp不允许变长数组，数组长度必须在编译时已知
        int len = days.size();
        //动态规划中我是要获取其最小值
        vector<int> dp(len,INT_MAX);
        dp[len-1] = min(costs[0],min(costs[1],costs[2]));
        for(int idx=len-2;idx>=0;idx--){
            for(int k=0;k<3;k++){
                int j = idx;
                while(j<days.size() && days[j]<=days[idx]+durations[k]-1){
                    j++;
                }
                if(j==days.size()){
                    dp[idx] = min(dp[idx],costs[k]);
                }else{
                    dp[idx] = min(dp[idx],costs[k]+dp[j]);
                }
            }
        }
        return dp[0];
        //缓存表
        //vector<int> memery(days.size(),-1);
        //return minFromIndex(days,costs,0,memery);
    }

    //原来这里之前是使用的
    int minFromIndex(vector<int>& days,vector<int>& costs,int startIndex,vector<int>& memory){
        if(startIndex==days.size()){
            return 0;
        }
        if(memory[startIndex]!=-1){
            return memory[startIndex];
        }
        //climits 头文件是默认包含的
        int ans = INT_MAX;
        for(int idx=0;idx<3;idx++){
            int j = startIndex;
            //只要days[j]在免费范围内，就增加
            while(j<days.size() && days[j] <= days[startIndex]+durations[idx]-1){
                j++;
            }
            ans = min(ans,costs[idx]+ minFromIndex(days,costs,j,memory));
        }
        //缓存答案
        memory[startIndex] = ans;
        return ans;
    }

};

int main(){
    int b = 2;
    int a = 1+b==2;
    cout<<a<<endl;
}