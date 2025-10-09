//
// Created by hsl on 2025/9/23.
//

#ifndef ALGORITHM_PRACTICE_BPLUSTREE_H
#define ALGORITHM_PRACTICE_BPLUSTREE_H
#include<vector>
using namespace std;
enum class IndexPageType {INVALID_INDEX_PAGE = 0,LEAF_PAGE,INTERNAL_PAGE};

class BaseNode{
protected:
    IndexPageType page_type_;//0 invalid 1internal leaf 2 leafnode
    int size_;// 当前关键字数量
    int max_size_;//键值对个数，max_size_+1为最大孩子数
    vector<int> keys;//关键字索引

public:
    BaseNode(){};
    BaseNode (int max_size);
    auto is_leaf()->bool;

    void set_size_(int s);

    virtual int get_size_();

    void incr();

    void decl();

    vector<int>& get_keys();

    virtual auto split()->shared_ptr<BaseNode> = 0;//说明是纯虚函数，派生类必须提供实现，否则不能被实例化

    virtual void merge(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> other) = 0;

//    auto insert(shared_ptr<BaseNode> root,int key,int value)->shared_ptr<BaseNode>;

//    virtual void update_after_split(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> left,shared_ptr<BaseNode> right) = 0;
    virtual void borrow_from_left_sibling(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> lsibling) = 0;

    virtual void borrow_from_right_sibling(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> rsibling) = 0;
};


class InternalNode :public BaseNode  {
private:
    vector<shared_ptr<BaseNode>> children;
public:
    explicit InternalNode(int max_size);

    auto split()->shared_ptr<BaseNode>;

    void update_after_split(shared_ptr<InternalNode> parent,int pos,shared_ptr<BaseNode> left,shared_ptr<BaseNode> right);

    auto get_pointers()->vector<shared_ptr<BaseNode>>&;

    void merge(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> other);

    void borrow_from_left_sibling(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> lsibling);

    void borrow_from_right_sibling(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> rsibling);
};

class LeafNode :public BaseNode,public enable_shared_from_this<LeafNode>{
private:
    vector<int> data;
public:
    shared_ptr<LeafNode> prev = nullptr;
    shared_ptr<LeafNode> next = nullptr;
    explicit LeafNode(int max_size);
    auto split()->shared_ptr<BaseNode>;
//    void update_after_split(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> left,shared_ptr<BaseNode> right);
    auto get_data()->vector<int>&;

    void merge(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> other);

    void borrow_from_left_sibling(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> lsibling);

    void borrow_from_right_sibling(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> rsibling);
};

class BPlusTree{
    friend class BaseNode;
    friend class LeafNode;
    friend class InternalNode;
private:
    shared_ptr<BaseNode> root;
    static int DEGREE;//阶，每个节点最大孩子节点数
public:
    explicit BPlusTree(int degree);
    void insert(int key,int value);
    void remove(int key);
    void remove_recursively(shared_ptr<BaseNode> root,int key);
        void query(int key);
//    auto split_node(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> child)->shared_ptr<BaseNode>;
    void insert_nofull(shared_ptr<BaseNode> root,int key,int value);
    void level_print();
    auto index_(vector<int>& list,int target,int left,int right)->int;
};

#endif //ALGORITHM_PRACTICE_BPLUSTREE_H
