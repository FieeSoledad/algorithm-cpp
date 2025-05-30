//
// Created by hsl on 2025/1/20.
//

#include<iostream>
#include<vector>
#include<string>
using namespace std;

class Solution {
public:
    int minimumTimeToInitialState(string word, int k) {
        int strlen = word.size();
        vector<int> z(strlen, 0);
        z[0] = strlen;
        for (int i = 1, c = 1, r = 1, len; i < strlen; i++) {
            len = (r > i) ? min(r - i, z[i - c]) : 0;
            while (i + len < strlen && word[i + len] == word[len]) {
                len++;
            }
            if (i + len > r) {
                r = i + len;
                c = i;
            }
            z[i] = len;
        }

        int ans = 0;
        for (int idx = k, step = 1; idx < strlen; idx += k, step += 1) {
            if (z[idx] == strlen - idx) {
                return step;
            }
        }
        //向上取整
        return (strlen + k - 1) / k;
    }
};

//https://leetcode.cn/problems/minimum-time-to-revert-word-to-initial-state-ii/
//每秒钟做操作移除前k个字符和后添加k个任意字符，问最快多久恢复
//扩展KMP算法的应用
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin>>s;
    Solution solution;
    solution.minimumTimeToInitialState(s,2);
    return 0;
}
