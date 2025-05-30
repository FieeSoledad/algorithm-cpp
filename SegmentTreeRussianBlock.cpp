#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;


class Solution {
public:
    //离散化后就是只需要这么多
    //TODO 这里为什么非得使用static 非静态的，是实例化对象的成员
    static const int MAXN = 2001;
    vector<int> arr;
    //用于SegmentTree
    //思考一波为什么需要change和update几个数组，需要表示是否存在懒信息
    int max_value[MAXN<<2];
    //change这个数组可能没那么有用诶，为什么不直接使用max_value这个数组呢
    int change[MAXN<<2];
    bool update[MAXN<<2];

    vector<int> fallingSquares(vector<vector<int>>& positions) {
        //离散化去重后的数量
        int n = collect(positions);
        build(1,n,1);

        vector<int> ans;
        int max_ = 0,l,r,h;
        for(int idx=0;idx<positions.size();idx++){
            l = get_rank(n,positions[idx][0]);
            //这里的离散化还是有点奇怪诶
            r = get_rank(n,positions[idx][0]+positions[idx][1]-1);
            h = query(l,r,1,n,1)+positions[idx][1];
            max_ = max(max_,h);
            ans.push_back(max_);
            update_h(l,r,1,n,1,h);
        }
        return ans;
    }

    /**
     * 将线段树jobl-jobr区间的最大值进行更新
     * @param jobl
     * @param jobr
     * @param left
     * @param right
     * @param v
     */
    void update_h(int jobl,int jobr,int left,int right,int index,int v){
        if(jobl<=left && jobr>=right){
            lazy_change(index,v);
        }else{
            down(index);
            int mid = (left+right)>>1;
            if(jobl<=mid) {
                update_h(jobl,jobr,left,mid,index<<1,v);
            }
            if(jobr>mid){
                update_h(jobl,jobr,mid+1,right,index<<1|1,v);
            }
            //哦，就是更新当前的内容。可是当前任务不需要从子节点中获取
            up(index);
        }
    }

    //二分查找找排名
    int get_rank(int n,int v){
        int ans = 0;
        int l = 0,r = n-1,m;
        while(l<=r){
            m = (l+r)/2;
            if(arr[m]>v){
                r = m-1;
            }else if(arr[m]<v){
                l = m+1;
            }else if(arr[m]==v){
                ans = m;
                break;
            }
        }
        //得到的是下标，排名还需要+1
        return ans+1;
    }

    //构建线段树
    /**
     *
     * @param left
     * @param right
     * @param i 数组内的下标
     */
    void build(int left,int right,int i){
        if(left<right){
            int mid = (left+right)/2;
            build(left,mid,i<<1);
            build(mid+1,right,i<<1|1);
        }
        max_value[i] = 0;
        change[i] = 0;
        update[i] = 0;
    }

    void lazy_change(int index,int v){
        max_value[index] = v;
        change[index] = v;
        update[index] = true;
    }

    //懒信息下推
    void down(int index){
        if(update[index]){
            //如果确实有懒信息
            lazy_change(index<<1,change[index]);
            lazy_change(index<<1|1,change[index]);
            //没有懒信息了。
            update[index] = false;
        }
    }

    void up(int index){
        max_value[index] = max(max_value[index<<1], max_value[index<<1|1]);
    }

    //线段树查询
    int query(int jobl,int jobr,int left,int right,int index){
        if(jobl<=left && jobr>=right){
            //将当前区间全部返回
            return max_value[index];
        }else{
            //懒信息下推
            down(index);
            int ans = 0;
            int mid = (left+right)>>1;
            if(jobl<=mid){
                ans = max(ans,query(jobl,jobr,left,mid,index<<1));
            }
            if(jobr>mid){
                ans = max(ans, query(jobl,jobr,mid+1,right,index<<1|1));
            }
            return ans;
        }
    }

    int collect(vector<vector<int>>& positions){
        for(int idx=0;idx<positions.size();idx++){
            arr.push_back(positions[idx][0]);
            arr.push_back(positions[idx][0]+positions[idx][1]-1);
        }
        //使用stl算法库中的算法进行排序
        sort(arr.begin(),arr.end());
        int n = 0;
        for(int idx=1;idx<arr.size();idx++){
            if(arr[n]!=arr[idx]){
                arr[++n] = arr[idx];
            }
        }
        return n+1;
    }
};

//题目类似于俄罗斯方块
//https://leetcode.cn/problems/falling-squares
//给出1000组数据，left_limit是10**8这个规模，长度len是10**6规模
//离散化解决
//4*10**8个int型 肯定会出现MLE
//第一次写至少对了一大半不是吗？
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    //明天开始重刷算法吧还是。真是无聊之极啊
    Solution s;
    //通过列表表达式赋值
    vector<vector<int>> v = {{1,2},{2,3},{6,1}};
    vector<int> ans = s.fallingSquares(v);
    for(int item:ans){
        cout<<item<<" "<<endl;
    }
    return 0;
}