//
// Created by hsl on 2024/11/29.
//
#include<iostream>
#include<climits>//INT_MIN等宏
#include<cstring>
using namespace std;
typedef long long ll;

//AVL树有序表专题，O(logn)的时间复杂度
//四种失衡情况，两种旋转操作
//递归检查
//静态数组实现

//总共十万次操作，可能都是插入操作
const int MAXN =  100001;
//下一个空间编号
int cnt = 0;
//整棵树的头节点编号
int head = 0;
//子树的插入的key的总数（不去重）
int subtree_size[MAXN];
int key[MAXN];
int avl_left[MAXN];
int avl_right[MAXN];
//节点key的计数
int key_count[MAXN];
//子树高度
int subtree_height[MAXN];

//旋转后维护信息
void up(int i){
    //subtree_size需要维护
    //subtree_height需要维护
    subtree_size[i] = subtree_size[avl_left[i]]+subtree_size[avl_right[i]]+key_count[i];
    subtree_height[i] = max(subtree_height[avl_left[i]],subtree_height[avl_right[i]])+1;
}

//左旋
//以i节点为头节点的树左旋，返回左旋hou头节点的空间编号
int leftRotate(int i){
    int r = avl_right[i];
    //接受右孩子的左子树
    avl_right[i] = avl_left[r];
    //右孩子的左孩子现在指向i
    avl_left[r] = i;
    //调整信息，i节点和右孩子的环境都变化了
    //因为信息是从子树收集的，所以这里也有顺序要求
    up(i);
    up(r);
    return r;
}

//右旋
int rightRotate(int i){
    int l = avl_left[i];
    avl_left[i] = avl_right[l];
    avl_right[l] = i;
    up(i);
    up(l);
    return l;
}

//检查此空间子树的平衡情况
//
int maintain(int i){
    int left_height = subtree_height[avl_left[i]];
    int right_height = subtree_height[avl_right[i]];

    if (left_height-right_height>1){//LL LR
        //TODO avl_left 写成avl_right是第二个错误
        if(subtree_height[avl_left[avl_left[i]]]>=subtree_height[avl_right[avl_left[i]]]){//LL
            i = rightRotate(i);
        }else {//LR型
            //先进行一次左旋，再进行一次右旋转
            avl_left[i] = leftRotate(avl_left[i]);
            i = rightRotate(i);
        }
        //TODO 平衡条件检查，这也是一个错误
    }else if(right_height-left_height>1){
        if(subtree_height[avl_right[avl_right[i]]]>=subtree_height[avl_left[avl_right[i]]]){//RR型
            i = leftRotate(i);
        }else{//RL型
            //先进行一次右旋，再进行左旋
            avl_right[i] = rightRotate(avl_right[i]);
            i = leftRotate(i);
        }
    }
    return i;
}

//返回树的头节点编号
//i：在以i为头节点的树上插入num
int add_key(int i,int num){
    if(i==0){
        //这里不做失衡检查，在递归回溯时检查，子树失衡得在父节点才能知道
        cnt++;
        key[cnt] = num;
        key_count[cnt] = 1;
        subtree_size[cnt] = 1;
        subtree_height[cnt] = 1;
        return cnt;
    }
    if(key[i]==num){
        key_count[i]++;
    }else if(key[i]>num){
        avl_left[i] = add_key(avl_left[i],num);
    }else if(key[i]<num){
        avl_right[i] = add_key(avl_right[i],num);
    }
    //汇总信息
    up(i);
    //检查是否失衡
    return maintain(i);
}

//新增节点
void add_key(int num){
    head = add_key(head,num);
}

int smaller_num(int i,int num){
    if(i==0){
        return 0;
    }
    if(key[i]>=num){
        return smaller_num(avl_left[i],num);
    }else{
        return subtree_size[avl_left[i]]+key_count[i]+ smaller_num(avl_right[i],num);
    }
}

//查看排名
//小于num的个数+1
int ot_rank(int num){
    return smaller_num(head,num)+1;
}

//当前子树排名第几
int index(int i,int idx){
    if(subtree_size[avl_left[i]]>=idx){
        return index(avl_left[i],idx);
    }else if(subtree_size[avl_left[i]]+key_count[i]<idx){
        return index(avl_right[i],idx-subtree_size[avl_left[i]]-key_count[i]);
    }else{
        return key[i];
    }
}

//查询排名第几
int index(int i){
    return index(head,i);
}

//查询前驱
int pre(int i,int num){
    if(i==0) {
        return INT_MIN;
    }
    //查前驱，肯定是小于的吧
    if(key[i]>=num){
        return pre(avl_left[i],num);
    }else{
        return max(key[i],pre(avl_right[i],num));
    }
}

//查询后继
int post(int i,int num){
    if(i==0){
        return INT_MAX;
    }
    if(key[i]<=num){
        return post(avl_right[i],num);
    }else{
        return min(key[i], post(avl_left[i],num));
    }

}


//删除最左节点
int removeMostLeft(int i,int mostLeft){
    //如何断连
    if(i==mostLeft){
        //没有左树了，但是还有右树
        return avl_right[i];
    }else{
        avl_left[i] = removeMostLeft(avl_left[i],mostLeft);
        up(i);
        return maintain(i);
    }
}

int remove_key(int i,int num){
    if(num>key[i]){
        avl_right[i] = remove_key(avl_right[i],num);
    }else if(num<key[i]){
        avl_left[i] = remove_key(avl_left[i],num);
    }else{
        if(key_count[i]>1){
            key_count[i]--;
        }else{
            //检查是叶节点还是非叶节点
            if(avl_left[i]==0 && avl_right[i]==0){//叶节点
                //为什么这里可以直接返回呢，因为当前i空间没有信息需要维护
                return 0;
            }else if(avl_left[i]==0 && avl_right[i]!=0){//左树为空
                i = avl_right[i];
            }else if(avl_left[i]!=0 && avl_right[i]==0){
                i = avl_left[i];
            }else{
                //寻找右子树的最左节点代替，不破坏二叉树的性质
                int mostLeft = avl_right[i];
                while(avl_left[mostLeft]!=0){
                    mostLeft = avl_left[mostLeft];
                }
                avl_right[i] = removeMostLeft(avl_right[i],mostLeft);
                avl_left[mostLeft] = avl_left[i];
                avl_right[mostLeft] = avl_right[i];
                i = mostLeft;
            }
        }
    }
    //为什么这里需要维护信息，size会变化，高度也有可能会变化
    up(i);
    return maintain(i);
}

//删除节点的过程
//叶子节点，非叶子节点（节点替换的过程）
void remove_key(int num){
    //检查是否存在
    if(ot_rank(num)!=ot_rank(num+1)){
        //树的结构是有可能变化的
        head = remove_key(head,num);
    }
}

void init(){
    cnt = 0;
    head = 0;
    memset(key,0,sizeof(key));
    memset(subtree_height,0,sizeof(subtree_height));
    memset(subtree_size,0,sizeof(subtree_size));
    memset(avl_left,0,sizeof(avl_left));
    memset(avl_right,0,sizeof(avl_right));
    memset(key_count,0,sizeof(key_count));
}


//平衡树模板题 https://www.luogu.com.cn/problem/P3369
int main(){
    init();
    int n;
    cin>>n;
    //cout<<" n is "<<n<<endl;
    for(int idx=0;idx<n;idx++){
        int opt,x;
        cin>>opt>>x;
        //cout<<opt<<" "<<x<<endl;
        if(opt==1){
            //TODO 第一个错误是没即时更新head
            add_key(x);
        }else if(opt==2){
            remove_key(x);
        }else if(opt==3){
            cout<<ot_rank(x)<<endl;
        }else if(opt==4){
            cout<<index(x)<<endl;
        }else if(opt==5){
            cout<<pre(head,x)<<endl;
        }else{
            cout<<post(head,x)<<endl;
        }
    }
    return 0;
}