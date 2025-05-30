//
// Created by hsl on 2024/11/6.
//


/**
* 矩阵中最长递增路径
*/

#include<vector>
using namespace std;

//图的上下左右搜索
vector<int> moved = {-1,0,1,0,-1};

bool validPosition(vector<vector<int>>& matrix,int i,int j){
    return i>=0&&j>=0&& i<matrix.size()&& j<matrix[0].size();
}
//从某处出发
//其实也不依赖pre
int ansFromOneIndex(vector<vector<int>>& matrix,int i,int j,vector<vector<int>>& memory){
    //越界了
    if(!validPosition(matrix,i,j)){
        return 0;
    }
    if(memory[i][j]>0){
        return memory[i][j];
    }
    int next = 0;
    int len = 1;
    for(int idx=0;idx<4;idx++){
        if(validPosition(matrix,i+moved[idx],j+moved[idx+1]) && matrix[i][j]<matrix[i+moved[idx]][j+moved[idx+1]])
            next = max(next, ansFromOneIndex(matrix,i+moved[idx],j+moved[idx+1],memory));
    }
    len+=next;
    memory[i][j] = len;
    return len;
}
class Solution{
public:
    int longestIncreasingPath(vector<vector<int>>& matrix){
        //不断更新
        int ans = 1;
        int n = matrix.size();
        int m = matrix[0].size();
        vector<vector<int>> memory(n,vector<int>(m,-1));
        for(int i=0;i<matrix.size();i++){
            for(int j=0;j<matrix[0].size();j++){
                ans = max(ans, ansFromOneIndex(matrix,i,j,memory));
            }
        }
        return ans;
    }
};