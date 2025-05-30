//
// Created by hsl on 2024/11/7.
//
/**
* 矩阵和种能够被K整除的路径
 *
 * 确实直接自顶向下的递归必定是超时的，使用记忆化搜索
*/

#include<vector>
using namespace std;

const int mod = 1000000007;
class Solution {
public:
    int numberOfPaths(vector<vector<int>>& grid, int k) {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<vector<int>>> memory(m,vector<vector<int>>(n,vector<int>(k+1,-1)));
        return dfs(grid,0,0,k,0,memory)%mod;
    }

    int dfs(vector<vector<int>>& grid,int i,int j,int k,int m,vector<vector<vector<int>>>& memory){
        if(i==grid.size()-1 && j==grid[0].size()-1){
            return grid[i][j]%k == m ?1:0;
        }
        if(memory[i][j][k]>=0){
            return memory[i][j][m];
        }
        int need = 0;
        if(grid[i][j]%k <=m){
            need = m-grid[i][j]%k;
        }else{
            need = k+m-grid[i][j]%k;
        }
        int ans = 0;
        if(i<grid.size()-1){
            ans+= dfs(grid,i+1,j,k,need,memory);
            ans%=mod;
        }
        if(j<grid[0].size()-1){
            ans+= dfs(grid,i,j+1,k,need,memory) % mod;
        }
        //看清楚参数
        memory[i][j][m] = ans;
        return ans;
    }
};