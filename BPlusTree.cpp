//
// Created by hsl on 2025/9/23.
//
#include<iostream>
#include<vector>
#include<memory>
#include<queue>
#include"BPlusTree.h"

using namespace std;

int BPlusTree::DEGREE = 5;

BaseNode::BaseNode(int max_size):max_size_(max_size){
    keys.reserve(max_size);
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
    this->children.reserve(max_size+1);
}

LeafNode::LeafNode(int max_size): BaseNode(max_size){
    this->page_type_ = IndexPageType::LEAF_PAGE;
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
    vector<int>& keys = parent->get_keys();
    for(int idx=parent->get_size_();idx>pos;idx--){
        keys[idx] = keys[idx-1];
    }

    vector<shared_ptr<BaseNode>>& pointers = parent->get_pointers();
    for(int idx=parent->get_size_()+1;idx>pos;idx--){
        pointers[idx+1] = pointers[idx];
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


shared_ptr<BaseNode> LeafNode::split() {
    auto new_child = make_shared<LeafNode>(BPlusTree::DEGREE-1);

    //1 移动关键字
    //2 移动数据
    //3 调整元素个数

    int move_num = this->size_ - this->size_/2;

    vector<int>& new_child_keys = new_child->get_keys();
    for(int idx=0;idx<move_num;idx++){
        new_child_keys[idx] = this->keys[idx+this->size_/2];
    }

    vector<int>& data = new_child->get_data();
    for(int idx=0;idx<move_num;idx++){
        data[idx] = this->data[idx+this->size_/2];
    }

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
        int pos = root->get_size_();
        vector<int>& keys = root->get_keys();
        while(pos>=1 && key<keys[pos-1]){
            keys[pos] = keys[pos-1];
            pos-=1;
        }
        keys[pos] = key;
        root->incr();//节点pairs数量+1
    }else{//索引节点，沿着路径向下查找，并沿途检查需要分裂的满元素节点
        auto current = dynamic_pointer_cast<InternalNode>(root);
        int pos = current->get_size_();
        auto keys = current->get_keys();
        while(pos>=1 && key<keys[pos-1]){//为什么从后往前找呢
            pos--;
        }

        auto pointers = current->get_pointers();
        if(pointers[pos]->get_size_() == BPlusTree::DEGREE){//元素个数达到最大，节点分裂
            //节点分裂
            auto new_child = pointers[pos]->split();
            //新的孩子插入原位
            current->update_after_split(current,pos,new_child,pointers[pos]);
            if(key>=keys[pos]){
                pos+=1;
            }
        }
        insert_nofull(current,key,value);//递归插入

    }

}

void BPlusTree::insert(int key, int value) {
    if(this->root == nullptr){
        return;
    }
    //满了
    if(this->root->get_size_()==DEGREE){
        //如果是叶子节点
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
 * 层级打印，调试专用
 * 使用队列
 * #include<queue>
 */
void BPlusTree::level_print() {
    queue<shared_ptr<BaseNode>> q;
    q.push(this->root);
    if(!q.empty()){
        cout<<"[";
        auto current = q.front();
        auto keys = current->get_keys();
        for(int idx=0;idx<current->get_size_();idx++){
            cout<<keys[idx]<<" ";
        }
        cout<<"]";
        if(!current->is_leaf()){
            auto current_internal = dynamic_pointer_cast<InternalNode>(current);
            auto pointers = current_internal->get_pointers();
            for(int idx=0;idx<=current_internal->get_size_();idx++){
                q.push(pointers[idx]);
            }
        }
        cout<<endl;//一层打印完后的换行符
    }
}

/**
 * 初始化b+树
 * @param degree b+树的阶，即每个内部节点最多多少个孩子节点
 */
BPlusTree::BPlusTree(int degree){
    //初始化度数
    this->DEGREE = degree;
    //初始化根节点
    this->root = make_shared<LeafNode>(this->DEGREE-1);
}


//B+树的实现
//全部对根节点特殊处理
//************* 增加分析   **************
//分为节点满或者不满
//（1）节点满，分裂，传递左孩子在父节点的指针索引位置
//（2）节点不满，调用非满节点插入
//（a）在叶子节点找到，直接新增
//（b）沿路径遇见孩子的关键字数量如果满，直接分裂


///************  删除分析   **************
//*
//在删除B树节点时，为了避免回溯，当遇到需要合并的节点时就立即执行合并，B树的删除算法如下：从root向叶子节点按照search规律遍历：
//（1）  如果target在叶节点x中，则直接从x中删除target，情况（2）和（3）会保证当再叶子节点找到target时，肯定能借节点或合并成功而不会引起父节点的关键字个数少于t-1。
//（2）  如果target在分支节点x中：
//（a）  如果x的左分支节点y至少包含t个关键字，则找出y的最右的关键字prev，并替换target，并在y中递归删除prev。
//（b）  如果x的右分支节点z至少包含t个关键字，则找出z的最左的关键字next，并替换target，并在z中递归删除next。
//（c）  否则，如果y和z都只有t-1个关键字，则将targe与z合并到y中，使得y有2t-1个关键字，再从y中递归删除target。
//（3）  如果关键字不在分支节点x中，则必然在x的某个分支节点p[i]中，如果p[i]节点只有t-1个关键字。
//（a）  如果p[i-1]拥有至少t个关键字，则将x的某个关键字降至p[i]中，将p[i-1]的最大节点上升至x中。
//（b）  如果p[i+1]拥有至少t个关键字，则将x个某个关键字降至p[i]中，将p[i+1]的最小关键字上升至x个。
//（c）  如果p[i-1]与p[i+1]都拥有t-1个关键字，则将p[i]与其中一个兄弟合并，将x的一个关键字降至合并的节点中，成为中间关键字。
//*
//*/F
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    BPlusTree bPlusTree(5);
    bPlusTree.insert(100,10);
    bPlusTree.level_print();
    return 0;
}