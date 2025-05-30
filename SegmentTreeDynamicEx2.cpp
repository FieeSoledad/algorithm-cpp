//
// Created by hsl on 2025/1/18.
//
#include<iostream>
#include<cstring>
using namespace std;

class CountIntervals {
public:
    static const int LIMIT = 10000;
    static const int MAXM = 100000;
    int n = 1e9;
    int cnt = 1;
    int left_node[LIMIT] = {0};
    int right_node[LIMIT] = {0};
    //应该不需要懒信息
    int range_sum[LIMIT] = {0};
    CountIntervals() {
        //默认构造函数好像是没什么需要的
        std::memset(left_node,0,sizeof(left_node));
        std::memset(right_node,0,sizeof(right_node));
        std::memset(range_sum,0,sizeof(range_sum));
        cnt = 1;
    }

    void up(int i){
        range_sum[i] = range_sum[left_node[i]]+range_sum[right_node[i]];
    }

    void range_add(int jobl,int jobr,int l,int r,int i){
        //TODO 这是隐藏极深的bug，是由于没有懒信息产生的。因为没有懒信息range_sum[]全为0，实际上却不为0
        if(range_sum[i]==r-l+1){
            return;
        }
        if(jobl<=l && jobr>=r){
            range_sum[i] = r-l+1;
        }else{
            int mid = (l+r)>>1;
            if(jobl<=mid){
                if(left_node[i]==0){
                    left_node[i] = ++cnt;
                }
                range_add(jobl,jobr,l,mid,left_node[i]);
            }
            if(jobr>mid){
                if(right_node[i]==0){
                    right_node[i] = ++cnt;
                }
                range_add(jobl,jobr,mid+1,r,right_node[i]);
            }
            up(i);
        }
    }

    int query(int jobl,int jobr,int l,int r,int i){

        if(jobl<=l && jobr>=r){
            return range_sum[i];
        }else{
            int mid = (l+r)>>1;
            int ans = 0;
            if(jobl<=mid){
                if(left_node[i]!=0){
                    ans+= query(jobl,jobr,l,mid,left_node[i]);
                }
            }
            if(jobr>mid){
                if(right_node[i]!=0){
                    ans+= query(jobl,jobr,mid+1,r,right_node[i]);
                }
            }
            return ans;

        }
    }

    void add(int left, int right) {
        range_add(left,right,1,n,1);
    }

    int count() {
        return range_sum[1];
    }
};

//https://leetcode.cn/problems/count-integers-in-intervals/
//开点线段树
//原来这边经常出现栈溢出啊。
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    CountIntervals c;
    cout<<c.count()<<endl;
    c.add(8,43);
    c.add(13,16);
    c.add(26,33);
    c.add(28,36);
    c.add(29,37);
    cout<<c.count()<<endl;
    c.add(34,46);
    c.add(10,23);
    return 0;
}