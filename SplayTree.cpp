//
// Created by hsl on 2024/12/4.
//

#include<iostream>
#include<climits>
using namespace std;

const int MAXN = 100000+1;
int cnt=0;
int head=0;
int key[MAXN];
int left_node[MAXN];
int right_node[MAXN];
int subtree_size[MAXN];
int father[MAXN];

void up(int i){
    subtree_size[i] = subtree_size[left_node[i]]+subtree_size[right_node[i]]+1;
}

//判断此节点是父亲节点的什么节点
int lr(int i){
    //右节点返回1
    return right_node[father[i]]==i?1:0;
}

//将i节点上旋一次
void rotate(int i){
    int f = father[i],g = father[f],soni = lr(i),sonf = lr(f);
    if(soni==1){//是右节点
        right_node[f] = left_node[i];
        if(right_node[f]!=0){
            father[right_node[f]]=f;
        }
        left_node[i] = f;
    }else{//是左节点
        left_node[f] = right_node[i];
        if(left_node[f]!=0){
            father[left_node[f]]=f;
        }
        right_node[i] = f;
    }
    if(g!=0){
        if(sonf==1){
            right_node[g] = i;
        }else{
            left_node[g] = i;
        }
    }
    father[i] = g;
    father[f] = i;
    up(f);
    up(i);
}

/**
 * 将i节点提根到goal的子节点
 * 分为一字型上提，Z字型上提，和上提
 * @param i
 * @param goal
 */
void splay(int i,int goal){
    int f = father[i],g = father[f];
    while(f!=goal){
        if(g!=goal){
            if(lr(i)==lr(f)){
                //一字型，先父亲，再提当前
                rotate(f);
            }else{
                rotate(i);
            }
        }
        //如果爷爷是目标节点，只要当前节点上升一次
        rotate(i);
        f = father[i];
        g = father[f];
    }
    //如果是根部
    if(goal==0){
        head = i;
    }
}

//添加节点
void add_key(int num){
    //插入之后，将我们的节点提到最顶部
    key[++cnt] = num;
    subtree_size[cnt] = 1;
    if(head==0){
        head = cnt;
    }else{
        int f=0,i = head,son=0;
        while(i!=0){
            if(num>=key[i]){
                f = i;
                i = right_node[i];
                son = 1;
            }else{
                f = i;
                i = left_node[i];
                son = 0;
            }
        }
        if(son==1){
            right_node[f] = cnt;
        }else{
            left_node[f] = cnt;
        }
        father[cnt] = f;
        //提根
        splay(cnt,0);
    }
}

//实现排名方法
int get_rank(int num){
    int i = head;
    int last = i;
    int small_num = 0;
    while(i!=0){
        last = i;
        if(key[i]>=num){

            i = left_node[i];
        }else{
            small_num+=subtree_size[left_node[i]]+1;
            i = right_node[i];
        }
    }
    //提根
    splay(last,0);
    return small_num+1;
}

int find_rank(int rank){
    int i = head;
    while(i!=0){
        if(subtree_size[left_node[i]]+1==rank){
            return i;
        }else if(subtree_size[left_node[i]]>=rank){
            i = left_node[i];
        }else{
            rank-=subtree_size[left_node[i]]+1;
            i = right_node[i];
        }
    }
    //正常是不会走到这一步的
    return 0;
}

//删除这个节点，先进行提根，笑死我嘞
void remove_key(int num){
    int kth = get_rank(num);
    if(kth!= get_rank(num+1)){
        int i = find_rank(kth);
        //提根
        splay(i,0);
        if(left_node[i]==0){
            head =  right_node[i];
        }else if(right_node[i]==0){
            head = left_node[i];
        }else{
            int j = find_rank(kth+1);
            splay(j,i);
            //抛弃i节点
            left_node[j] = left_node[i];
            father[left_node[j]] = j;
            up(j);
            head = j;
        }
        //抛弃i节点
        father[head] = 0;
    }
}

//查询排名第idx的节点
int index(int idx){
    int i = find_rank(idx);
    splay(i,0);
    return key[i];
}

//查询前驱节点
int pre(int num){
    int i = head;
    int ans = INT_MIN;
    int last = i;
    while(i!=0){
        last = i;
        if(key[i]>=num){
            i = left_node[i];
        }else{
            ans = max(ans,key[i]);
            i = right_node[i];
        }
    }
    //擦汗寻
    splay(last,0);
    return ans;
}

int post(int num){
    int i = head;
    int ans = INT_MAX;
    int last = head;
    while(i!=0){
        last = i;
        if(key[i]<=num){
            i = right_node[i];
        }else{
            ans = min(ans,key[i]);
            i = left_node[i];
        }
    }
    splay(last,0);
    return ans;
}

//伸展树Splay Tree，发明者：Robert Tarjan
//提根操作
//三种情况的提根操作：依赖左旋和右旋操作
//一步调整
//一字型，父亲节点向上，当前节点向上，这样可以消减高度
//之字型，当前节点向上两次，这样才可以一次上升两个位置
//不管什么样的长链，底层节点提升到头节点，高度几乎会减少一半
//https://www.luogu.com.cn/problem/P3369

//不使用词频压缩，维护关系：左<头<=右
//重要方法：find(rank),问题是为什么不使用递归，非得用循环
//插入：插入后提根
//删除操作，不断提根
//插入删除都有提根操作，同时要保证每种查询都有提根操作

//rank方法：为什么要把一个不空的点提到根部呢，每次操作都要有的提根操作
//Splay树慎用递归，因为可能出现长链爆栈
int main(){
    ios::sync_with_stdio(false);
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
        }else {
            cout << post(num) << endl;
        }
    }
    return 0;
}