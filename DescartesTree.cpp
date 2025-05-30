//
// Created by hsl on 2024/12/1.
//

#include<iostream>
#include<vector>
using namespace std;
typedef long long ll;

//数据量在10的七次方
const int MAXN = 10000000+1;
int left_node[MAXN];
int right_node[MAXN];
int diff_arr[MAXN];
int n;
int stack_[MAXN];


void build(){
    for(int i=1,pos = 0,top=0;i<=n;i++){
        //弹栈
        //要保证大压小
        while(pos>0 && diff_arr[stack_[pos]] > diff_arr[i]){
            pos--;
        }
        //可能要将原本的右树截断
        if(pos>0){
            right_node[stack_[pos]] = i;
        }
        //有可能我一个节点都没有移动诶
        if(pos<top){
            left_node[i] = stack_[pos+1];
        }
        //把当前内容入栈
        stack_[++pos] = i;
        top = pos;
    }
}

//笛卡尔树的实现借助单调栈
//实现思路：先按照key值排序进行遍历，出栈时一定保证栈内元素的key值小于当前，作为左孩子，保证是二叉搜索树
//入栈保证value大压小，最后出栈的节点作为孩子节点，这样保证小根堆的性质。
//线性的时间复杂度，当然可能取决于排序
//https://www.luogu.com.cn/problem/P5854
int main(){
    //io处理，应该是能提速的诶
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n;
    for(int idx=1;idx<=n;idx++){
        cin >> diff_arr[idx];
    }
    build();
    ll ans1 = 0;
    ll ans2 = 0;
    for(int idx=1;idx<=n;idx++){
        ans1^=(ll)idx*(left_node[idx]+1);
        ans2^=(ll)idx*(right_node[idx]+1);
    }
    cout<<ans1<<" "<<ans2<<endl;
    return 0;

//    这个是自动vec.size()返回的是size_t，一种无符号整数
//    vector<int> v;
//    v.push_back(123);
//    v.push_back(456);
//    for(auto i=v.size()-1;i>=0;i--){
//        cout<<v[i]<<endl;
//    }

}
