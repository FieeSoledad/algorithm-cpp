//
// Created by hsl on 2025/1/21.
//

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;
typedef long long ll;

const int MAXN = 10000000+1;
const int mod = 19930726;
//manacher问题相关
char ss[MAXN<<1];
int p[MAXN<<1];
int length;
int n,K;
int cnt[MAXN];
int max_ = 1;

//获取处理串
void manacherss(string& s,char* ss){
    length = 2*s.size()+1;
    for(int i=0,j=0;i<length;i++){
        ss[i] = (i&1)==0?'#':s[j++];
    }
}


ll my_pow(ll base,ll ex){
    ll ans = 1;
    while(ex>0){
        if((ex&1)==1){
            ans = ans*base;
        }
        base = base*base;
        ex = ex>>1;
    }
    return ans;
}

/**
 * 使用优先级队列时间复杂度是O(k*logK)
 * @param s
 * @return
 */
int manacher(string& s){
    manacherss(s,ss);
    for(int i=0,c=0,r=0,len;i<length;i++) {
        len = r > i ? min(r - i, p[2 * c - i]) : 1;
        while (i + len < length && i - len >= 0 && ss[i + len] == ss[i - len]) {
            len++;
        }
        if (i + len > r) {
            r = i + len;
            c = i;
        }
        p[i] = len;
        //收集奇数长度的数量
        if ((len-1 & 1) == 1) {
            cnt[len-1]++;
            max_ = max(max_,len-1);
        }
    }
    //
    int res = 1;
    int ps = 0;
    for(int idx=max_;idx>=1 && K>0;idx-=2){
        ps+=cnt[idx];
        if(idx==1 && K>cnt[idx]){
            return -1;
        }
        res = (res*my_pow(idx,min(K,ps))) % mod;
        K-=cnt[idx];
    }
    return res;
}

//https://www.luogu.com.cn/problem/P1659
//查找女生的和谐小团体
//并且需要奇数个
//堆也不能有诶。
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>K;
    string s;
    cin>>s;
    int ans = manacher(s);
    cout<<ans<<endl;
    return 0;
}
