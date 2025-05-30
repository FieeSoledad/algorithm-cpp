//
// Created by hsl on 2025/1/21.
//

#include<iostream>
#include<string>
using namespace std;

class Solution{
public:
    static const int MAXN = 1000+1;
    //处理串
    char ss[MAXN<<1];
    int p[MAXN<<1];
    int length;
    int countSubstrings(string s) {
        manacherss(s,ss);
        int ans = 0;
        for(int i=0,c=0,r=0,len;i<length;i++){
            len = r>i?min(r-i,p[2*c-i]):1;
            while(i+len<length && i-len>=0 && ss[i+len]==ss[i-len]){
                len++;
            }
            if(i+len>r){
                r = i+len;
                c = i;
            }
            p[i] = len;
            ans+=p[i]/2;
        }
        return ans;
    }

    void manacherss(string s,char* ss){
        //处理串的长度
        length = 2*s.size()+1;
        for(int i=0,j=0;i<length;i++){
            ss[i] = (i&1)==0?'#':s[j++];
        }
    }
};



//https://leetcode.cn/problems/palindromic-substrings/
//需要掌握回文串的数量和回文半径的关系
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution solution;
    cout<<solution.countSubstrings("abc");
    return 0;
}
