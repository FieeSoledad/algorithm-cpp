//
// Created by hsl on 2024/11/7.
//

/**
* 骑士在棋盘上的存活概率
*/

/**
 * 存活概率：每个展开方向上的存活概率加权
 * 使用记忆化搜索解决
 */
#include<vector>
using namespace std;
class Solution{
public:
    double knightProbability(int n, int k, int row, int column) {
        //声明数组，但是没有分配内存
        //声明一个三维数组
        vector<vector<vector<double>>> memory(n,vector<vector<double>>(n,vector<double>(k+1,-1)));

        return getProbability(n,row,column,k,memory);
    }

    //递归返回
    double getProbability(int n,int i,int j,int k,vector<vector<vector<double>>>& memory){
        //出界了，直接返回0，宣告死亡
        if(!(i>=0 && j>=0 && i<n && j<n)){
            return 0;
        }
        //不用走了，百分百存活
        if(k<=0) {
            return 1;
        }
        if(memory[i][j][k]!=-1){
            return memory[i][j][k];
        }
        double ans = 0;
        ans+= getProbability(n,i-2,j-1,k-1,memory)/8;
        ans+= getProbability(n,i-1,j-2,k-1,memory)/8;
        ans+= getProbability(n,i+1,j-2,k-1,memory)/8;
        ans+= getProbability(n,i+2,j-1,k-1,memory)/8;
        ans+= getProbability(n,i-2,j+1,k-1,memory)/8;
        ans+= getProbability(n,i-1,j+2,k-1,memory)/8;
        ans+= getProbability(n,i+1,j+2,k-1,memory)/8;
        ans+= getProbability(n,i+2,j+1,k-1,memory)/8;
        memory[i][j][k] = ans;
        return ans;
    }

};