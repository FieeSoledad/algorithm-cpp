//
// Created by hsl on 2024/12/11.
//

#include<iostream>
#include<string>
#include<vector>
using namespace std;


/**
 * 对字符串求next数组返回
 * @param s
 * @param len
 * @return
 */
vector<int> next_array(string s){
    int len = s.size();
    if(len==1){
        vector<int> ans(1,0);
        return ans;
    }
    vector<int> ans(len,0);
    ans[0] = -1;
    //这是因为没有前后缀
    ans[1] = 0;
    int index = 2;
    //前缀的默认为
    int cn = 0;
    while(index<len) {
        if (s[index-1] == s[cn]) {
            ans[index++] = ++cn;
        } else if (cn > 0) {
            //为什么大于0就可以跳跃呢，如果是1呢？就是跳到0呗
            cn = ans[cn];
        } else {//当前最长前缀就是0了说明
            ans[index++] = 0;
        }
    }
    return ans;
}

/**
 * 判断s1中是否有s2
 * @param s1
 * @param s2
 * @return
 */
int kmp(string s1,string s2){
    int len1 = s1.size();
    int len2 = s2.size();
    vector<int> nextArr = next_array(s2);
    //比对的位置
    int x = 0,y = 0;
    while(x<len1 && y<len2){
        if(s1[x]==s2[y]){
            x++;
            y++;
        }else if(y==0){//此时第一个字母开始都比对失败了
            x++;
        }else{
            y = nextArr[y];
        }
    }
    return y==len2?x-y:-1;
}

//kmp的模板
//时间复杂度的证明比较有意思
int main(){
    string s1 = "aabaabaaf";
    string s2 = "aabaaf";
    cout<<kmp(s1,s2)<<endl;
    return 0;
}