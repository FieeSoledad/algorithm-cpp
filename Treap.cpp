//
// Created by hsl on 2024/12/2.
//

#include<iostream>
//使用其中cstdlib的rand()函数
#include<random>
#include<climits>
using namespace std;
typedef long long ll;

const int MAXN = 100000+1;
int cnt = 0;
int head=0;
int key[MAXN];
int key_count[MAXN];
int left_node[MAXN];
int right_node[MAXN];
int subtree_size[MAXN];
double priority[MAXN];

void up(int i){
    //从子树汇总信息
    subtree_size[i] = subtree_size[left_node[i]]+subtree_size[right_node[i]]+key_count[i];
}

//左旋和右旋都与AVL树的旋转操作不变
//以i为头节点的树做左旋
int leftRotate(int i){
    int r =  right_node[i];
    right_node[i] = left_node[r];
    left_node[r] = i;
    up(i);
    up(r);
    //返回这个新的头节点
    return r;
}

//以i为头节点的树做右旋
int rightRotate(int i){
    int l = left_node[i];
    //TODO 这里有个问题
    left_node[i] = right_node[l];
    right_node[l] = i;
    up(i);
    up(l);
    return l;
}

//思考一下这个如何发生调整呢
int add_key(int i,int num) {
    //需要新建节点的时候
    if (i == 0) {
        key[++cnt] = num;
        key_count[cnt] = 1;
        subtree_size[cnt] = 1;
        priority[cnt] = static_cast<double>(rand()) / RAND_MAX;
        return cnt;
    }
    if (key[i] == num) {//只需增加词频
        key_count[i] += 1;
        //TODO 之前没有维护
        subtree_size[i] += 1;
    } else if (num > key[i]) {
        right_node[i] = add_key(right_node[i], num);
        //先汇总信息，可能不发生旋转诶
        up(i);
        //左右孩子不可能为空诶，旋转也只可能发生一次
        if (right_node[i] != 0 && priority[right_node[i]] > priority[i]) {
            return leftRotate(i);
        }
    } else {
        left_node[i] = add_key(left_node[i], num);
        up(i);
        if (left_node[i] != 0 && priority[left_node[i]] > priority[i]) {
            return rightRotate(i);
        }
    }
    //TODO 这里之前缺少返回
    return i;
}

int small(int i,int num){
    if(i==0){
        return 0;
    }
    if(key[i]>=num){
        return small(left_node[i],num);
    }else{
        return subtree_size[left_node[i]]+key_count[i]+small(right_node[i],num);
    }
}


//返回树中比num小的数字的数量
int get_rank(int num){
    return small(head,num)+1;
}

//我实现的删除过程
int remove_key(int i,int num){
    if(key[i]==num){
        if(key_count[i]>1){
            key_count[i]-=1;
            //TODO 这个信息也是忘记维护了
            subtree_size[i]-=1;
            //TODO 这里之前缺少返回
            return i;
        }else{
            if(left_node[i]==0 && right_node[i]==0){
                return 0;
            }else if(left_node[i]==0 && right_node[i]!=0){
                return right_node[i];
            }else if(left_node[i]!=0 && right_node[i]==0){
                return left_node[i];
            }else{
                if(priority[left_node[i]]>priority[right_node[i]]){
                    i = rightRotate(i);
                    right_node[i] = remove_key(right_node[i],num);
                }else{
                    i = leftRotate(i);
                    left_node[i] = remove_key(left_node[i],num);
                }
                //汇总信息
                up(i);
                return i;

                 //删除也是递归地删除诶
                 //左右子树都不为空，使用下滤操作
                 //保存旧的头节点
//                 int temp = i;
//                 //这个是新的头节点
//                 i = priority[right_node[i]]>priority[left_node[i]]?right_node[i]: left_node[i];
//                 //缺少信息维护，这个确实用递归来比较好
//                 //这里是下旋过程
//                 while(left_node[temp]!=0 || right_node[temp]!=0){
//                     //cpp允许这些乱七八糟的隐式转换
//                     double lp = left_node[temp]!=0?priority[left_node[temp]]:-1;
//                     double rp = right_node[temp]!=0?priority[right_node[temp]]:-1;
//                     if(rp>lp){
//                         leftRotate(temp);
//                     }else{
//                         rightRotate(temp);
//                     }
//                 }
//                 return i;
            }
        }
    }else{
        if(num>key[i]){
            right_node[i] = remove_key(right_node[i],num);
        }else{
            left_node[i] = remove_key(left_node[i],num);
        }
        //TODO 这里之前忘记汇总信息了
        up(i);
        return i;
    }
}

//删除节点
void remove_key(int num){
    if(get_rank(num)!= get_rank(num+1)){
        head = remove_key(head,num);
    }
}

//新增节点
void add_key(int num){
    head = add_key(head,num);
}

int index(int i,int idx){
    if(subtree_size[left_node[i]]>=idx){
        return index(left_node[i],idx);
    }else if(subtree_size[left_node[i]]+key_count[i]>=idx){
        return key[i];
    }else{
        return index(right_node[i],idx-subtree_size[left_node[i]]-key_count[i]);
    }
}

//如何实现天梯赛的强者呢
//查询树中第idx的数字
int index(int idx){
    return index(head,idx);
}

int pre(int i,int num){
    if(i==0){
        return INT_MIN;
    }
    if(key[i]>=num){
        return pre(left_node[i],num);
    }else{
        return max(key[i],pre(right_node[i],num));
    }
}

//查询num的前驱
int pre(int num){
    return pre(head,num);
}

int post(int i,int num){
    if(i==0){
        return INT_MAX;
    }
    if(key[i]<=num){
        return post(right_node[i],num);
    }else{
        return min(key[i],post(left_node[i],num));
    }
}

//查询num的后继
int post(int num){
    return post(head,num);
}


//树堆Treap
//利用随机化避免数据倾斜
//我现在实现的是使用大根堆的树堆
//https://www.luogu.com.cn/problem/P3369
int main(){
    //解除同步
    ios::sync_with_stdio(false);
    //cin自动刷新cout取消
    cin.tie(nullptr);
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
        }else if(opt==6){
            cout<<post(num)<<endl;
        }
    }
    return 0;
}