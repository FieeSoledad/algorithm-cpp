//
// Created by hsl on 2024/11/30.
//

#include<iostream>
#include<random>
#include<cstring>
#include<climits>
using namespace std;
typedef long long ll;

//手撕跳跃表
//实现思想也是比较先进的
//小就增加层数，大就确定层数

//最大层链表限制
const int MAXL = 20;
const int MAXN = 100000+1;
//空间计数
int cnt = 0;
int key[MAXN];
int key_count[MAXN];
int level_map[MAXN];// 记忆每个插入的节点的层高
//维护指针
int next_node[MAXN][MAXL+1];
//维护指针跨越的元素个数，左开右闭区间
int len[MAXN][MAXL+1];

//丢骰子决定层数
int random_level(){
    int l = 1;
    //这个是生成浮点数
    uniform_real_distribution<> dis(0,1);
    mt19937 gen;
    while(dis(gen)<0.5){
        l++;
    }
    return min(MAXL,l);
}


//只是找空间编号罢了
int find_key(int i,int level,int num){
    while(next_node[i][level]!=0 && key[next_node[i][level]]<num){//别忘了，这里是next的语义
        i = next_node[i][level];
    }
    if(level==1){
        //如果是0，那么这里也不可能等于num啊,就先这样吧，可能的，元素可能就是0哦
        if(next_node[i][level]!=0 && key[next_node[i][level]]==num){
            return next_node[i][level];
        }else{//说明没有找到
            return 0;
        }
    }
    //去下一层找
    //但是在这里有没可能已经找到了呢，为什么非得到第一层才返回？
    return find_key(i,level-1,num);
}

//维护词频和指针跨越长度
void add_count(int i,int level,int num){
    while(next_node[i][level]!=0 && key[next_node[i][level]]<num){//但是为什么是小于num，这是要维护前一个指针的跨越长度信息
        i = next_node[i][level];
    }
    if(level==1){// 添加词频
        key_count[next_node[i][level]]+=1;
    }else{
        add_count(i,level-1,num);
    }
    //当前层的长度需要增加
    len[i][level]+=1;
}

//插入空间编号为cnt的节点
//难度在哪里？一个指针可能分裂成为两个指针，两指针跨越长度的修正需要递归获取
//指针也可能不分裂
//返回的是什么？在[1,200]插入160时，返回(1,160]的元素数量
int add_node(int i,int level,int cnt){
    //确实是比较困难的诶，有尾递归的实现线索二叉树吗
    int right_cnt = 0;
    while(next_node[i][level] != 0 && key[next_node[i][level]] < key[cnt]){
        right_cnt += len[i][level];
        i = next_node[i][level];
    }
    if(level == 1){//为什么到了level1就能确定了呢
        next_node[cnt][level] = next_node[i][level];
        next_node[i][level] = cnt;
        //长度
        len[cnt][level] = key_count[next_node[cnt][level]];
        //这是新增节点，当然只有一个
        len[i][level] = 1;
        //之所以不需要加1，是因为上层
        return right_cnt;
    }else{
        int down_cnt = add_node(i,level-1,cnt);
        if(level>level_map[cnt]){
            //指针无需分裂，指针跨越长度直接自增
            len[i][level]+=1;
        }else{
            next_node[cnt][level] = next_node[i][level];
            next_node[i][level] = cnt;
            //维护指针长度
            //原本长度+本次增加-中间区间的数字-本数字内容
            len[cnt][level] = len[i][level]+1-down_cnt-1;
            len[i][level] = down_cnt+1;
            return right_cnt+down_cnt;
        }
    }
}

//添加
//分为添加词频和新增节点
void add_key(int num){
    if(find_key(1,MAXL,num)!=0){//只需要添加词频，维护指针跳跃长度
        add_count(1,MAXL,num);
    }else{//添加节点
        cnt++;
        key[cnt] = num;
        key_count[cnt] = 1;
        //这个level数组有什么用呢，其实真的用处不大吧，只需要记住层高就行
        level_map[cnt] = random_level();
        add_node(1,MAXL,cnt);
    }
}

void decline_count(int i,int level,int num){
    //先右移到不能再右移为止
    while(next_node[i][level]!=0 && key[next_node[i][level]]<num){
        i = next_node[i][level];
    }
    if(level==1){
        //前面已经确定是有这个数的，所以这里next指针一定指向对应元素
        key_count[next_node[i][level]]-=1;
    }else{
        decline_count(i,level-1,num);
    }
    len[i][level]-=1;// 可是，可能这个节点根本就没有对应的层次，但是没关系，我们不会访问到
}

//删除节点，所以这里涉及的是指针合并，只有删除节点
//删除某个空间编号
//为什么这里传递的是空间编号，因为要获取某个空间的指针层高
//这个确实是比较好理解
void remove_node(int i,int level,int j){
    while(next_node[i][level]!=0 && key[next_node[i][level]]<key[j]){
        i = next_node[i][level];
    }
    if(level==1){
        next_node[i][level] = next_node[j][level];
        len[i][level]+=len[j][level]-1;
    }else{
        if(level>level_map[j]){
            len[i][level]-=1;
            remove_node(i,level-1,j);
        }else{
            next_node[i][level] = next_node[j][level];
            len[i][level]+=len[j][level]-1;
            remove_node(i,level-1,j);
        }
    }
}

void remove_key(int num){
    //TODO 这个错误是这里多了等号
    int j = find_key(1,MAXL,num);
    if(j!=0){
        if(key_count[j]>1){//只需改变词频
            decline_count(1,MAXL,num);
        }else{
            remove_node(1,MAXL,j);
        }
    }
}

//查询有多少个比num小
int small(int i,int level,int num) {
    int right_cnt = 0;
    while(next_node[i][level]!=0 && key[next_node[i][level]]<num){
        right_cnt+=len[i][level];
        i = next_node[i][level];
    }
    if(level==1){
        return right_cnt;
    }else{
        return right_cnt+ small(i,level-1,num);
    }
}
//获取num的排名
int get_rank(int num){
    return small(1,MAXL,num)+1;
}

//从i号空间向右，第idx的数
int index(int i,int level,int idx){
    //递归结束条件怎么说呢
    if(idx<=len[i][level] && idx>len[i][level]-key_count[next_node[i][level]]){
        //TODO 第一个错误，没有取值
        return key[next_node[i][level]];
    }else if(idx<=len[i][level]-key_count[next_node[i][level]]){
        return index(i,level-1,idx);
    }else{//这个肯定就是大于或等于了
        return index(next_node[i][level],level,idx-len[i][level]);
    }
}

//排名第idx的数
int index(int idx){
    //cout<<" query "<<idx<<endl;
    return index(1,MAXL,idx);
}


int pre(int i,int level,int num){
    while(next_node[i][level]!=0 && key[next_node[i][level]]<num){
        i = next_node[i][level];
    }
    if(level==1){
        return i==1?INT_MIN:key[i];
    }else{
        return pre(i,level-1,num);
    }
}

//获取前驱
//小于num的最大节点
int pre(int num){
    return pre(1,MAXL,num);
}

//获取后继
int post(int i,int level,int num){
    //当前层可能不能往右滑了，但是下一层可能还能哦
    while(next_node[i][level]!=0 && key[next_node[i][level]]<=num){//TODO 只有这一处能改成等号
        i = next_node[i][level];
    }
    if(level==1){
        //情况似乎没有考虑完全
        //如果next_node等于当前节点就不合适了
        return next_node[i][level]==0?INT_MAX:key[next_node[i][level]];
    }else{
        return post(i,level-1,num);
    }
}

//获取后继
int post(int num){
    return post(1,MAXL,num);
}

void build(){
    memset(key,0,sizeof (key));
    memset(key_count,0,sizeof (key_count));
    memset(level_map,0,sizeof (level_map));
    memset(next_node,0,sizeof (next_node));
    memset(len,0,sizeof (len));
    //一号空间给最小层
    cnt = 1;
    key[cnt] = INT_MIN;
    //最小的拥有最高的层数
    level_map[cnt] = MAXL;
}

//https://www.luogu.com.cn/problem/P3369
//这个跳跃表实现是否浪费存储指针的空间？
int main(){
    build();
    int n;
    cin>>n;
    //cout<<" n is "<<n<<endl;
    for(int idx=0;idx<n;idx++){
        int opt,x;
        cin>>opt>>x;
        if(opt==1){
            add_key(x);
        }else if(opt==2){
            remove_key(x);
        }else if(opt==3){
            cout<<get_rank(x)<<endl;
        }else if(opt==4){
            cout<<index(x)<<endl;
        }else if(opt==5){
            cout<<pre(x)<<endl;
        }else{
            cout<<post(x)<<endl;
        }
    }
    return 0;
}
