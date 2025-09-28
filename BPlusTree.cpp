////
//// Created by hsl on 2025/9/23.
////
#include<iostream>
#include<vector>
#include<memory>
#include<queue>
#include"BPlusTree.h"

using namespace std;

int BPlusTree::DEGREE = 5;

BaseNode::BaseNode(int max_size):max_size_(max_size){
    this->keys.resize(max_size);
    this->size_ = 0;
}
bool BaseNode::is_leaf(){
    return this->page_type_ == IndexPageType::LEAF_PAGE;
}
void BaseNode::set_size_(int s){
    this->size_ = s;
}

int BaseNode::get_size_(){
    return this->size_;
}
void BaseNode::incr(){
    this->size_+=1;
}

void BaseNode::decl(){
    this->size_-=1;
}

vector<int>& BaseNode::get_keys(){
    return this->keys;
}

InternalNode::InternalNode(int max_size):BaseNode(max_size){
    this->page_type_ = IndexPageType::INTERNAL_PAGE;
//    this->keys.reserve(max_size);//最大关键字数量
    this->children.resize(max_size+1);

}

LeafNode::LeafNode(int max_size): BaseNode(max_size){
    this->page_type_ = IndexPageType::LEAF_PAGE;
//    this->keys.reserve(max_size);//最大关键字数量
    this->data.resize(max_size);
}

vector<int>& LeafNode::get_data(){
    return this->data;
}

vector<shared_ptr<BaseNode>>& InternalNode::get_pointers(){
    return this->children;
}

void InternalNode::update_after_split(shared_ptr<InternalNode> parent,int pos,shared_ptr<BaseNode> left,shared_ptr<BaseNode> right){
    //0 parent空出pos+1位置
    //1 parent的[pos] = left
    //2 parent[pos+1] = right+1;
    //4 设置parent的key
    //5 关键字数量+1
    //key       [pos-1]      [pos]   [pos+1]  [pos+2]
    //pointers          [pos]    [pos+1]
    vector<int>& keys = parent->get_keys();
    for(int idx=parent->get_size_();idx>pos;idx--){
        keys[idx] = keys[idx-1];
    }

    vector<shared_ptr<BaseNode>>& pointers = parent->get_pointers();
    for(int idx=parent->get_size_();idx>pos;idx--){
        pointers[idx] = pointers[idx-1];
    }

    pointers[pos] = left;
    pointers[pos+1] = right;
    keys[pos] = right->get_keys()[0];

    parent->incr();//关键字数量加1
}


shared_ptr<BaseNode> InternalNode::split() {
    auto new_child = make_shared<InternalNode>(BPlusTree::DEGREE-1);

    //1 移动数据
    //2 移动指针
    //3 修改元素个数
    int move_num = this->size_ -  this->size_/2;

    vector<int>& new_child_keys = new_child->get_keys();
    for(int idx=0;idx<move_num;idx++){
        new_child_keys[idx] = this->keys[idx+this->size_/2];
    }

    vector<shared_ptr<BaseNode>>& pointers = new_child->get_pointers();
    for(int idx=0;idx<=move_num;idx++){//指针比关键集大一
        pointers[idx] = this->children[idx+this->size_/2];
    }

    this->set_size_(this->size_- move_num);
    new_child->set_size_(move_num);

    return new_child;
}

/**
 * 必要时合并两颗子树
 * @return
 */
void InternalNode::merge(shared_ptr<BaseNode> other) {
    //1移动关键字
    //2移动指针
    //3修改本对象的关键字数量
    //  [0][1]        [0][1]
    // [0][1][2]     [0][1][2]
    auto other_raw = dynamic_pointer_cast<InternalNode>(other);
    auto num1 = this->get_size_();
    auto num2 = other->get_size_();

}


void LeafNode::merge(shared_ptr<BaseNode> other) {

}

/**
 * 其中分裂出的new_child将会作为右子树
 * @return
 */
shared_ptr<BaseNode> LeafNode::split() {
    auto new_child = make_shared<LeafNode>(BPlusTree::DEGREE-1);

    //1 移动关键字
    //2 移动数据
    //3 调整元素个数

    int move_num = this->size_ - this->size_/2;

    //TODO 无法自动推导引用
    auto& new_child_keys = new_child->get_keys();
    for(int idx=0;idx<move_num;idx++){
        new_child_keys[idx] = this->keys[idx+this->size_/2];
    }

    auto& new_data = new_child->get_data();
    for(int idx=0;idx<move_num;idx++){
        new_data[idx] = this->data[idx+this->size_/2];
    }

    new_child->set_size_(move_num);
    this->set_size_(this->size_-move_num);

    //更新双向链表
    if(this->next != nullptr){
        new_child->next = this->next;
        this->next->prev = new_child;
    }
    new_child->prev = shared_from_this();//nb，这个可能需要好好研究一下
    this->next = new_child;

    return new_child;
}

//void LeafNode::update_after_split() {
//
//}




///**
// *
// * @param parent 新的父亲节点
// * @param pos 插槽
// * @param child 待分裂节点，满节点
// * @return
// */
//shared_ptr<BaseNode> BPlusTree::split_node(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> child){
//
//    //1 拷贝一半数据到新的子节点
//    //2 拷贝一半指针
//    //3 两个孩子节点的指针放入父节点的插槽
//    //4 调整父节点的关键字数组
//
//    auto new_child = child->split();
//
//    new_child->update_after_split(parent,pos,child,new_child);
////    auto child_ = dynamic_pointer_cast<InternalNode>(child);//左
//
//
//    int move_num = child->get_size_()/2;
//
//    vector<int>& left_child_keys = child->get_keys();
//}


/**
 *
 * @param root 当前节点不是满的
 * @param key
 * @param value
 */
void BPlusTree::insert_nofull(shared_ptr<BaseNode> root,int key,int value){
    if(root->is_leaf()){//如果是叶子节点，直接插入
        auto root_leaf = dynamic_pointer_cast<LeafNode>(root);
        int pos = root_leaf->get_size_();
        vector<int>& keys = root_leaf->get_keys();
        while(pos>=1 && key<keys[pos-1]){
            keys[pos] = keys[pos-1];
            pos-=1;
        }
        keys[pos] = key;
        vector<int>& data = root_leaf->get_data();
        data[pos] = value;
        root_leaf->incr();//节点pairs数量+1
    }else{//索引节点，沿着路径向下查找，并沿途检查需要分裂的满元素节点
        auto current = dynamic_pointer_cast<InternalNode>(root);
        int pos = current->get_size_();
        auto& keys = current->get_keys();
        while(pos>=1 && key<keys[pos-1]){//为什么从后往前找呢
            pos--;
        }

        auto& pointers = current->get_pointers();
        if(pointers[pos]->get_size_() == BPlusTree::DEGREE-1){//元素个数达到最大，节点分裂
            //节点分裂
            auto new_child = pointers[pos]->split();
            //新的孩子插入原位
            current->update_after_split(current,pos,pointers[pos],new_child);
            if(key>=keys[pos]){
                pos+=1;
            }
        }
        insert_nofull(pointers[pos],key,value);//递归插入
    }

}

void BPlusTree::insert(int key, int value) {
    if(this->root == nullptr){
        return;
    }
    //满了
    cout<<"insert. before root size is "<<this->root->get_size_()<<endl;
    if(this->root->get_size_()==DEGREE-1){//
        //如果是叶子节点
        cout<<"触发分裂，增长树高"<<endl;
        auto new_parent = make_shared<InternalNode>(BPlusTree::DEGREE-1);
        auto new_child = this->root->split();
        new_parent->update_after_split(new_parent,0,this->root,new_child);
        this->root = new_parent;
        this->insert_nofull(this->root,key,value);
    }else{
        this->insert_nofull(this->root,key,value);
        //需要返回什么吗？
    }
}
/**
 * 删除B+树的关键字
 * B+树的删除情况比B树少一种
 * @param key
 */
void BPlusTree::remove(int key){//为什么有必要进行特殊处理？因为需要修改根节点指针
    if(this->root->get_size_()==1 && !this->root->is_leaf()) {//非叶子节点
        auto internal_node = dynamic_pointer_cast<InternalNode>(this->root);
        auto& pointers = internal_node->get_pointers();
        auto left = pointers[0];
        auto right = pointers[1];

        if(left != nullptr && right!= nullptr && left->get_size_()==(DEGREE-1)/2 && right->get_size_()==(DEGREE-1)/2){
            //合并两颗子树
        }

    }
}




/**
 * 层级打印，调试专用
 * 使用队列
 * #include<queue>
 */
void BPlusTree::level_print() {
    queue<shared_ptr<BaseNode>> q;
    q.push(this->root);
    int size = q.size();
    while(!q.empty()){
        size-=1;
        cout<<"[";
        auto current = q.front();
        q.pop();
        auto& keys = current->get_keys();
        for(int idx=0;idx<current->get_size_();idx++){
            cout<<keys[idx]<<" ";
        }
        cout<<"]";
        if(!current->is_leaf()){
            auto current_internal = dynamic_pointer_cast<InternalNode>(current);
            auto& pointers = current_internal->get_pointers();
            for(int idx=0;idx<=current_internal->get_size_();idx++){
                q.push(pointers[idx]);
            }
        }
        if(size==0){
            size = q.size();
            cout<<endl;
        }
    }
}

/**
 * 初始化b+树
 * @param degree b+树的阶，即每个内部节点最多多少个孩子节点
 */
BPlusTree::BPlusTree(int degree){
    //初始化度数
    this->DEGREE = degree;
    //初始化根节点，一个叶子节点
    auto node = make_shared<LeafNode>(this->DEGREE-1);
    node->set_size_(0);
    node->prev = nullptr;
    node->next = nullptr;
    this->root = node;
}


//B+树的实现
//全部对根节点特殊处理
//************* 增加分析   **************
//分为节点满或者不满
//（1）节点满，分裂，传递左孩子在父节点的指针索引位置
//（2）节点不满，调用非满节点插入
//（a）在叶子节点找到，直接新增
//（b）沿路径遇见孩子的关键字数量如果满，直接分裂


///************  B树删除分析   **************
//*
//在删除B树节点时，为了避免回溯，当遇到需要合并的节点时就立即执行合并，B树的删除算法如下：从root向叶子节点按照search规律遍历：
//B树因为数据存储在内部节点和叶子节点，在内部节点被替代后就是被删除了，不需要递归删除target
//（1）  如果target在叶节点x中，则直接从x中删除target，情况（2）和（3）会保证当再叶子节点找到target时，肯定能借节点或合并成功而不会引起父节点的关键字个数少于t-1。
//（2）  如果target在分支节点x中：
//（a）  如果x的左分支节点y至少包含t个关键字，则找qw出y的最右的关键字prev，并替换target，并在y中递归删除prev。
//（b）  如果x的右分支节点z至少包含t个关键字，则找出z的最左的关键字next，并替换target，并在z中递归删除next。
//（c）  否则，如果y和z都只有t-1个关键字，则将target与z合并到y中，使得y有2t-1个关键字，再从y中递归删除target。
//（3）  如果关键字不在分支节点x中，则必然在x的某个分支节点p[i]中，如果p[i]节点只有t-1个关键字。
//（a）  如果p[i-1]拥有至少t个关键字，则将x的某个关键字降至p[i]中，将p[i-1]的最大节点上升至x中。
//（b）  如果p[i+1]拥有至少t个关键字，则将x个某个关键字降至p[i]中，将p[i+1]的最小关键字上升至x个。
//（c）  如果p[i-1]与p[i+1]都拥有t-1个关键字，则将p[i]与其中一个兄弟合并，将x的一个关键字降至合并的节点中，成为中间关键字。
//*
//*/F

///************  B+树删除分析   **************
//*
// B+树因为所谓的数据存储在叶子节点，内部节点并没有存储数据，关键字只作为路由，所以不需要删除内部节点的数据
//在删除B树节点时，为了避免回溯，当遇到需要合并的节点时就立即执行合并，B树的删除算法如下：从root向叶子节点按照search规律遍历：
//（1）  如果target在叶节点x中，则直接从x中删除target，情况（2）和（3）会保证当再叶子节点找到target时，肯定能借节点或合并成功而不会引起父节点的关键字个数少于t-1。
//（2）  如果关键字不在分支节点x中，则必然在x的某个分支节点p[i]中，如果p[i]节点只有t-1个关键字。
//（a）  如果p[i-1]拥有至少t个关键字，则将x的某个关键字降至p[i]中，将p[i-1]的最大节点上升至x中。
//（b）  如果p[i+1]拥有至少t个关键字，则将x个某个关键字降至p[i]中，将p[i+1]的最小关键字上升至x个。
//（c）  如果p[i-1]与p[i+1]都拥有t-1个关键字，则将p[i]与其中一个兄弟合并，将x的一个关键字降至合并的节点中，成为中间关键字。
//*
//*/F

//[100,101][102,103,104]

//决定了，插入使用提前避免的方式实现，删除使用递归回溯的方式实现
//烦死了，还是不要害怕递归的好呀。
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    //测试插入分裂
    BPlusTree bPlusTree(5);//最多四个关键字
    bPlusTree.insert(100,10);
    bPlusTree.insert(101,10);
    bPlusTree.insert(102,10);
    bPlusTree.insert(103,10);
    bPlusTree.insert(104,10);
    bPlusTree.insert(105,10);
    bPlusTree.insert(106,10);
    bPlusTree.level_print();
    //测试删除combination
    bPlusTree.remove(104);
    bPlusTree.remove(103);
    return 0;
}