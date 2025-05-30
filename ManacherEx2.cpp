//
// Created by hsl on 2025/1/21.
//
#include<iostream>
#include<string>
using namespace std;



class Solution{
public:
    static const int MAXN = 2000+1;
    char ss[MAXN<<1];
    int p[MAXN<<1];
    int length;
    int maxPalindromes(string s, int k) {
        manacherss(s,ss);
        int ans = 0;
        int left = 0;
        while((left = find(left,k))!=-1){
            ans++;
        }
        return ans;
    }

    //当l左边的不存在
    int find(int l,int k){
        for(int i=l,c=l,r=l,len;i<length;i++){
            len = r>i?min(r-i,p[2*c-i]):1;
            while(i-len>=l && i+len<length && ss[i-len]==ss[i+len]){
                len++;
                //回文串的长度一旦大于等于k，截断
                if(ss[i+len]!='#' && len-1>=k){
                    return i+len-1;
                }else if(ss[i+len]=='#' && ss[c]=='#' && len-1/2>=k){
                    return i+len;
                }else if(ss[i+len]=='#' && ss[c]!='#' && len-1/2>=k){
                    return i+len;
                }
            }
            if(i+len>r){
                r = i+len;
                c = i;
            }
            p[i] = len;
        }
        return -1;
    }

    void manacherss(string s,char* ss){
        length = 2*s.size()+1;
        for(int i=0,j=0;i<length;i++){
            ss[i] = (i&1)==0?'#':s[j++];
        }
    }
};

//https://leetcode.cn/problems/maximum-number-of-non-overlapping-palindrome-substrings/
//要求是不重叠
//需要对manacher算法进行一定的改写。
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution solution;
    cout<<solution.maxPalindromes("abaccdbbd",3);
    return 0;
}
