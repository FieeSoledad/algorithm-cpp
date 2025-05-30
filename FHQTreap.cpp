//
// Created by hsl on 2024/12/4.
//
#include<iostream>
#include<random>
using namespace std;

const int MAXN = 100000+1;
int head = 0;
int cnt = 0;
int key[MAXN];
//用于词频压缩
int key_count[MAXN];
int left_node[MAXN];
int right_node[MAXN];
int subtree_size[MAXN];
//随机优先级
double priority[MAXN];

//https://www.luogu.com.cn/problem/P3369
//基于split和merge实现的Treap，但是这个api是如何设计出来的呢？

void up(int i){
    subtree_size[i] = subtree_size[left_node[i]]+subtree_size[right_node[i]]+key_count[i];
}

/**
 * 将以cur为头节点的BST分裂为<=num和>num的两棵树
 * 这些树都是走过的树高的程度，所以行为是什么呢，
 * @param l <=num要设置的指针
 * @param r >num要设置的指针  这两个一开始都是0
 * @param cur
 * @param num
 */
void split(int l,int r,int cur,int num){
    if(cur==0){
        right_node[l] = left_node[r] = 0;
    }else{
        if(key[cur]<=num){
            right_node[l] = cur;
            //当前节点的左树肯定是属于l的，但是右树不一定
            split(cur,r,right_node[cur],num);
        }else if(key[cur]>num){
            left_node[r] = cur;
            split(l,cur,left_node[cur],num);
        }
        //如果汇总的信息是l和r会有什么问题？反正我们不会访问subtree_size[0]
        up(cur);
    }
}

/**
 * 将两个Treap合并成一个Treap
 * 已经确定了：l上的key值全都小于r上的key值，所以合并的时候看value值，也就是priority，要求Treap符合大根堆性质，谁大谁作头
 * @param l
 * @param r
 */
int merge(int l,int r){
    if(l==0 || r==0){
        return l+r;
    }
    if(priority[l]>=priority[r]){
        right_node[l] = merge(right_node[l],r);
        up(l);
        return l;
    }else {
        //TODO 这里传参要保证左树的key值小于右树
        left_node[r] = merge(l,left_node[r]);
        up(r);
        return r;
    }
}

//返回空间编号
int find_num(int i,int num){
    if(i==0){
        return 0;
    }
    if(num>key[i]){
        return find_num(right_node[i],num);
    }else if(num<key[i]){
        return find_num(left_node[i],num);
    }else{
        return i;
    }
}

//更新词频
void modify_count(int i,int num,int step){
    //TODO 下标访问
    if(num>key[i]){
        modify_count(right_node[i],num,step);
    }else if(num<key[i]){
        modify_count(left_node[i],num,step);
    }else{
        key_count[i]+=step;
    }
    up(i);
}

void add_key(int num){
    if(find_num(head,num)!=0){
        //增加词频
        modify_count(head,num,1);
    }else{
        //新增节点
        split(0,0,head,num);
        key[++cnt] = num;
        key_count[cnt] = 1;
        subtree_size[cnt] = 1;
        priority[cnt] = (double)rand()/RAND_MAX;
        //合并
        head = merge(merge(right_node[0],cnt),left_node[0]);
    }
}

void remove_key(int num){
    int i = find_num(head,num);
    if(i!=0){
        if(key_count[i]>1){
            modify_count(head,num,-1);
        }else{
            //删除节点
            split(0,0,head,num);
            int r = left_node[0];
            split(0,0,right_node[0],num-1);
            //合并，此时丢弃num所在的节点
            head = merge(right_node[0],r);
        }
    }
}

int small(int i,int num){
    if(i==0){
        return 0;
    }
    if(num<=key[i]){
        return small(left_node[i],num);
    }else{
        return subtree_size[left_node[i]]+key_count[i]+small(right_node[i],num);
    }
}

int get_rank(int num){
    //TODO 这里忘加1了
    return small(head,num)+1;
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

//获取num的排名
int index(int num){
    return index(head,num);
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

//查询其前驱
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

//查询其后继
int post(int num){
    return post(head,num);
}

//FHQ Treap的词频压缩实现方式
//https://www.luogu.com.cn/problem/P3369
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin>>n;
    for(int idx=1;idx<=n;idx++){
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
