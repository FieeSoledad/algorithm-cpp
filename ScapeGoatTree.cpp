//
// Created by hsl on 2024/11/30.
//

#include<iostream>
using namespace std;
typedef long long ll;

//平衡因子
const float ALPHA = 0.7;
const int MAXN = 100000+1;
//下一个空间编号
int cnt=0;
//整棵树的头节点编号
int head = 0;
int key[MAXN];
int key_count[MAXN];
int left_node[MAXN];
int right_node[MAXN];
//子树上的节点数，用于调整平衡
int diff[MAXN];
//子树上所有的数字量
int subtree_size[MAXN];
//重构时收集节点使用
int collect[MAXN];
int ci;
//重构时使用
int top;
int father;
int side;

int init(int num){
    cnt+=1;
    key[cnt] = num;
    key_count[cnt] = 1;
    left_node[cnt] = 0;
    right_node[cnt] = 0;
    diff[cnt] = 1;
    subtree_size[cnt] = 1;
    return cnt;
}

void up(int i){
    diff[i] = diff[left_node[i]]+diff[right_node[i]]+(key_count[i]>0?1:0);
    subtree_size[i] = subtree_size[left_node[i]]+subtree_size[right_node[i]]+key_count[i];
}

bool is_balance(int i){
    return ALPHA*diff[i]>=max(diff[left_node[i]],diff[right_node[i]]);
}

/**
 * @param i
 * @param f
 * @param s
 * @param num
 */
int add_key(int i, int f, int s, int num){
    //Segmentation Fault,无限递归堆栈溢出，打印函数参数debug
    if(i==0){
        //需要新增节点
        //初始化
        return init(num);
        //连接，为什么不在父节点做连接呢？添加一个return int？应该也是可以的
    }else{
        if(key[i]==num){
            //只需要增加词频
            key_count[i]+=1;
            subtree_size[i]+=1;
        }else if(num>key[i]){
            //连接这个过程还是在父节点做了吧
            right_node[i] = add_key(right_node[i],i,2,num);
        }else if(num<key[i]){
            left_node[i] = add_key(left_node[i],i,1,num);
        }
        //从子节点修正信息
        up(i);
        if(!is_balance(i)){
            top = i;
            father = f;
            side = s;
        }
        return i;
    }
}

void preorder(int i){
    if(i!=0){
        preorder(left_node[i]);
        if(key_count[i]>0){
            collect[++ci] = i;
        }
        preorder(right_node[i]);
    }
}


int build(int l,int r){
    if(l>r){//问题是这里为什么是>，如果是等于直接返回当前内容行不行呢，不行，因为当前节点的信息也要修正
        return 0;
    }
    int mid = (l+r)/2;
    int h = collect[mid];
    left_node[h] = build(l,mid-1);
    right_node[h] = build(mid+1,r);
    //从子树修正信息
    up(h);
    return h;
}

//重构
void rebuild(){
    if(top!=0){//需要重构
        ci = 0;
        preorder(top);
        if(ci>0){//问题是ci怎么可能小于0呢
            if(father==0){
                head = build(1,ci);
            }else if(side==1){
                //TODO 这里真的是重大错误
                left_node[father] = build(1,ci);
            }else if(side==2){
                right_node[father] = build(1,ci);
            }
            //top节点的信息无需修正
        }
    }
}

//新增节点
void add_key(int num){
    top = father = side = 0;
    //TODO 第一个错误是没设置好head
    head = add_key(head,0,0,num);
    rebuild();
}


//获取更小的数量
//看一眼就知道这里有无限的递归诶
int small(int i,int num){
    if(i==0){
        return 0;
    }
    if(key[i]>=num){
        return small(left_node[i],num);
    }else{
        return subtree_size[left_node[i]]+key_count[i]+ small(right_node[i],num);
    }
}

//获取num的排名
int get_rank(int num){
    return small(head,num)+1;
}


//
int removeMostLeft(int i,int mostLeft){
    if(i==mostLeft){
        return right_node[i];
    }else{
        left_node[i] = removeMostLeft(left_node[i],mostLeft);
        up(i);
        //不检查平衡是对的吗？
        return i;
    }
}


/**
 * 删除以i为头节点的树上的num
 * 删除的过程也存在重构行为
 * @param i
 * @param f
 * @param s
 * @param num
 */
int remove_key(int i,int f,int s,int num){
    //在这里递归结束
    if(key[i]==num){
        if(key_count[i]>1){
            //扣减词频
            key_count[i]-=1;
            //这里我已经维护了本节点的信息了，所以直接返回没问题
            subtree_size[i]-=1;
            return i;
        }else{
            //需要删除节点
            if(left_node[i]==0 && right_node[i]==0){//是叶子节点
                return 0;
            }else if(left_node[i]!=0 && right_node[i]==0){//右树为空
                //为什么这里不能直接返回呢
                i = left_node[i];
                //TODO 这里少了一个等号
            }else if(left_node[i]==0 && right_node[i]!=0){//左树为空
                i = right_node[i];
            }else if(left_node[i]!=0 && right_node[i]!=0){//左右都不为空
                int mostLeft = right_node[i];
                //找到最左节点
                while(left_node[mostLeft]!=0){
                    mostLeft = left_node[mostLeft];
                }
                //删除右树最左节点
                right_node[i] = removeMostLeft(right_node[i],mostLeft);
                //替换上最左节点
                left_node[mostLeft] = left_node[i];
                right_node[mostLeft] = right_node[i];
                i = mostLeft;
            }
        }
    }else {
        //递归未结束呢，别口嗨，脏数据的实现可能性是什么呢。这影
        if(num>key[i]){
            right_node[i] = remove_key(right_node[i],i,2,num);
        }else if(num<key[i]){
            left_node[i] = remove_key(left_node[i],i,1,num);
        }
    }
    up(i);
    if(!is_balance(i)){
        top = i;
        father = f;
        side = s;
    }
    return i;
}

//删除某个节点，我要做到什么呢
//我要拒绝实现延迟删除策略
void remove_key(int num){
    if(get_rank(num) != get_rank(num+1)){
        top = father = side=0;
        head = remove_key(head,0,0,num);
        //删除过程也可能导致重构
        rebuild();
    }
}

int index(int i,int idx){
    if(subtree_size[left_node[i]]>=idx){
        return index(left_node[i],idx);
        //TODO 这里是我傻逼了
    }else if(subtree_size[left_node[i]]+key_count[i]>=idx){
        return key[i];
    }else{
        return index(right_node[i],idx-subtree_size[left_node[i]]-key_count[i]);
    }
}

int index(int idx){
    return index(head,idx);
}

//为什么我只能使用递归写出来？
int pre(int i,int num){
    if(i==0){
        return INT_MIN;
    }else{
        if(key[i]>=num){
            return pre(left_node[i],num);
        }else{
            return max(key[i], pre(right_node[i],num));
        }
    }
}

//获取前驱
int pre(int num){
    return pre(head,num);
}

int post(int i,int num){
    if(i==0){
        return INT_MAX;
    }else{
        //TODO 写错索引更无敌
        if(key[i]<=num){
            //TODO 写错函数签名也是无敌
            return post(right_node[i],num);
        }else{
            return min(key[i], post(left_node[i],num));
        }
    }
}

//获取后继
int post(int num){
    return post(head,num);
}

//https://www.luogu.com.cn/problem/P3369
//增删节点：删除节点时不会即时删除，不过为什么不使用最右节点的最左节点替代呢?确实很有问题诶？这个操作的时间复杂度也是log(n)诶
//节点被删除却不计入diff，会实实在在影响查询性能诶。

//替罪羊树,扩展性相对较弱

//写出一个build通过中序遍历的数组构建树，返回新树头节点的编号
//我实在不理解，为什么这个add方法还需要维护过程中的左右节点这些内容,所有递归回溯的时候是上层覆盖下层，在add完成后再进行重构
//我真的服了，怎么是这个重构函数写错了
int main(){
    int n;
    cin>>n;
    for(int idx=0;idx<n;idx++){
        int opt,num;
        cin>>opt>>num;
        if(opt==1){
            add_key(num);
        }else if(opt==2){
            remove_key(num);
        }else if(opt==3){
            cout<<get_rank(num)<<endl;
        }else if(opt==4){
            cout<<index(num)<<endl;
        }else if(opt==5){
            cout<<pre(num)<<endl;
        }else{
            cout<<post(num)<<endl;
        }
    }
    return 0;
}
