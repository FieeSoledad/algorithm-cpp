//
// Created by hsl on 2024/9/16.
//
#include<iostream>
#include<vector>
#include<map>
using namespace std;

/**
 * 简简单单的两数之和
 */
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        //声明就是初始化了
        map<int,int> mapper;
        vector<int> ans(2);
        //看来vector也是有重载[的
        for(int idx=0;idx<nums.size();idx++){
            if(mapper.count(target-nums[idx])>0){
                ans[0] = idx;
                ans[1] = mapper[target-nums[idx]];
            }else{
                mapper.insert(pair<int,int>(nums[idx],idx));
            }
        }
        return ans;
    }
};