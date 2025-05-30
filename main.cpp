#include<iostream>
#include<vector>
#include<list>
#include<stack>
#include<queue>
#include<set>
#include<map>
using namespace std;
//不同平台的实现
typedef long long ll;
void vectorDemo(){
    vector<int> v1;
    vector<int> v2(10);//创建一个大小为10的向量
    vector<int> v3(10,5);//创建并填充
    vector<int> v4(v3.begin(),v3.end());//创建指向
    vector<int> v5(v4);

    v4.push_back(10);
    v4.pop_back();
    v4.insert(v4.begin(),1);//插入
    //v4.erase()//清空指定的迭代器的内容
    //v4.clear()//清空所有
    //v4.size()
    vector<int>::iterator it;
    for(it = v3.begin();it!=v3.end();it++){
        cout<<*it<<endl;
    }
}

//这个跟Java中不太一样，返回值为true，则将第一个元素放在前面
int compare(const int &a,const int &b){
    return a>b;
}

void listDemo(){
    list<int> l1;
    list<int> l2(10);

    l1.empty();
    l1.push_front(1);
    l1.pop_front();
    l1.push_back(1);
    l1.pop_back();

    //声明了就是创建了
    list<int> l3;
    l3.push_back(1);
    l3.push_back(2);

    l3.sort();
    for(list<int>::iterator it = l3.begin();it!=l3.end();it++){
        cout<<*it<<endl;
    }
    l3.sort(compare);
    for(list<int>::iterator it = l3.begin();it!=l3.end();it++){
        cout<<*it<<endl;
    }
}


void stackDemo(){
    stack<int> s;
    cout<<s.empty()<<endl;
    s.push(10);
    cout<<s.empty()<<endl;
    cout<< s.top()<<endl;
}

//这么看来这个其实是一个双端队列
void queueDemo(){
    queue<int> q;
    q.push(10);
    q.push(20);
    cout<<q.front()<<endl;
    cout<<q.back()<<endl;
    q.pop();
    cout<<q.front()<<endl;
}

//这个比较特殊，就是需要结构体的方法重载，跟Java是一样的
struct cmp{
    bool operator()(int &a,int &b){
        return a>b;
    }
};

/**
 * 优先级队列的声明是 type、container、func，其中底层容器类型container需要支持随机访问操作
 */
void priorityQueueDemo(){
    //默认是一个最大堆
    priority_queue<int> pq;
    pq.push(10);
    pq.push(20);
    cout<<pq.top()<<endl;
    //现在是一个最小堆
    priority_queue<int,vector<int>, cmp> q;
    q.push(20);
    q.push(10);
    cout<<q.top()<<endl;
}

/**
 * 巨大的疑问是为什么set.find()查找原本不存在的元素还会返回1呢
 */
void setDemo() {
    set<int> set;
    set.insert(10);
    //set.erase(10);删除元素
    cout << *set.find(10) << endl;
    cout << *set.find(9) << endl;
}

//这个我们能够获取到value，但是如何如何获取到key呢？
/**
 * map是有序的，底层是红黑树结构
 * unordered_map是无序的，底层是哈希表结构
 */
void mapDemo(){
    map<char,int> mapper;
    mapper.insert(pair<char,int>('a',10));
    cout<<endl;
    cout<< mapper.find('a')->second<<endl;
    cout<<mapper['a']<<endl;
    //cout<<mapper.end()<<endl;
}

//multi-set multi-map
//set存储有序的重复的键
//map适用于存储有重复键的一对多映射
int main() {
    //关闭cpp和c标准流的同步
    ios::sync_with_stdio(false);
    //关闭cin自动刷新cout
    cin.tie(nullptr);
    //vectorDemo();
    //listDemo();
    //stackDemo();
    //queueDemo();
    //priorityQueueDemo();
    //setDemo();
    //mapDemo();
    return 0;
}


