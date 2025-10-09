//
// Created by hsl on 2025/9/27.
//

#include<iostream>
#include<vector>
#include<memory>
#include<queue>
#include "BPlusTreeRecursive.h"

using namespace std;

int BPlusTree::DEGREE = 0;

int index_(vector<int>& list,int target,int left,int right){
    int candidate = -1;
    while(left<=right){
        int mid = (left+right)>>1;
        if(target>=list[mid]){
            candidate = mid;
            left = mid+1;
        }else{
            right = mid-1;
        }
    }
    return candidate;
}

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
    this->prev = nullptr;
    this->next = nullptr;
}

vector<int>& LeafNode::get_data(){
    return this->data;
}

vector<shared_ptr<BaseNode>>& InternalNode::get_pointers(){
    return this->children;
}


/**
 * 必要时合并两颗子树
 * @return
 */
//void InternalNode::merge(shared_ptr<BaseNode> parent, int pos, shared_ptr<BaseNode> other) {
//    auto other_raw = dynamic_pointer_cast<InternalNode>(other);
//    auto num1 = this->get_size_();
//    auto num2 = other->get_size_();
//}

/**
 * 非叶子节点的插入
 * @param root
 * @param key
 * @param value
 * @return
 */
insert_result InternalNode::insert(shared_ptr<BaseNode> root, int key, int value){
    int target = index_(this->get_keys(),key,0, this->get_size_()-1);

    auto res = this->children[target+1]->insert(this->children[target+1],key,value);
    if(res.right != nullptr){//发生分裂
        if(this->get_size_()==BPlusTree::DEGREE-1){//本节点一样满，本节点一样分裂
//            (this->get_size_()+1)//2//  9+1/2 = 5 8+1/2 = 4
            auto left_num = this->get_size_()/2;
            auto right_num = this->get_size_()-left_num;

            auto new_node = make_shared<InternalNode>(BPlusTree::DEGREE-1);
            auto& new_keys = new_node->get_keys();
            auto& new_children = new_node->get_pointers();

            target = target+1;//变为新关键字的目标位置
            if(target == left_num){//新键刚好可以放中间
                 new_children[0] = res.right;
                 //拷贝关键字
                 //拷贝孩子节点
                 for(int idx=0;idx<right_num;idx++){
                     new_keys[idx] = this->keys[left_num+idx];
                     new_children[1+idx] = this->children[left_num+1+idx];
                 }
                 //调整元素数量
                 this->set_size_(left_num);
                 new_node->set_size_(right_num);

                 return insert_result{res.middle_key,shared_from_this(),new_node};

            }else if(target<left_num){//新键放在分裂后的左节点
                //拷贝关键字
                //拷贝孩子节点
                for(int idx=0;idx<right_num;idx++){
                    new_keys[idx] = this->keys[left_num+idx];
                    new_children[idx] = this->children[left_num+idx];
                }
                new_children[right_num] = this->children[this->get_size_()];
                //调整节点关键字数量
                this->set_size_(left_num);
                new_node->set_size_(right_num);

                //左树空出关键字插槽
                for(int idx = this->get_size_();idx>target;idx--){
                    this->keys[idx] = this->keys[idx-1];
                }
                //左树空出指针插槽
                for(int idx = this->get_size_();idx>target+1;idx--){
                    this->children[idx] = this->children[idx-1];
                }
                this->keys[target] = res.middle_key;
                this->children[target+1] = res.right;

                return insert_result{this->keys[this->get_size_()],shared_from_this(),new_node};
            }else {//新键放在分裂后的右节点
                //
                for (int idx = 0; idx < target - left_num - 1; idx++) {
                    new_keys[idx] = this->keys[left_num + 1 + idx];
                }
                for (int idx = 0; idx < target - left_num; idx++) {//
                    new_children[idx] = this->children[left_num + 1 + idx];
                }
                //target在新的数组中的索引为target - left_num-1;
                new_keys[target - left_num - 1] = res.middle_key;
                new_children[target - left_num] = res.right;

                //继续拷贝剩下的
                for (int idx = target - left_num; idx < right_num; idx++) {
                    new_keys[idx] = this->keys[left_num+idx];
                    new_children[idx+1] = this->children[left_num+idx+1];
                }

                //调整节点元素个数
                this->set_size_(left_num);
                new_node->set_size_(right_num);

                return insert_result{this->keys[this->get_size_()], shared_from_this(), new_node};
            }
        }else{//本节点未满，不分裂
            //空出槽位
            //插入
            //调整元素数量
            for(int idx = this->get_size_();idx>target+1;idx--){
                this->keys[idx] = this->keys[idx-1];
            }
            for(int idx = this->get_size_()+1;idx>target+2;idx--){
                this->children[idx] = this->children[idx-1];
            }
            this->keys[target+1] = res.middle_key;
            this->children[target+2] = res.right;

            this->incr();
            return insert_result{0, nullptr, nullptr};
        }
    }
    return insert_result{0, nullptr, nullptr};
}

/**
 * 叶子节点插入
 * @param root
 * @param key
 * @param value
 * @return
 */
insert_result LeafNode::insert(shared_ptr<BaseNode> root, int key, int value){
    int target = index_(this->get_keys(),key,0, this->get_size_()-1);

    if(target>=0 && this->keys[target]==key){//不允许有重复的键
        return insert_result{0, nullptr, nullptr};
    }

    if(this->get_size_() < this->max_size_){//此页面未满
        target = target+1;//变为目标槽位
        //空出槽位
        //插入
        //调整元素数量
        for(int idx=this->get_size_();idx>target;idx--){
            this->keys[idx] = this->keys[idx-1];
            this->data[idx] = this->data[idx-1];
        }
        this->keys[target] = key;
        this->data[target] = value;

        this->incr();

        return insert_result{0, nullptr, nullptr};

    }else {//此页已满，发生分裂
        auto left_num = (this->get_size_()+1) / 2;
        auto right_num = this->get_size_() - left_num + 1;

        auto new_node = make_shared<LeafNode>(BPlusTree::DEGREE - 1);
        auto &new_keys = new_node->get_keys();
        auto &new_data = new_node->get_data();

        target = target + 1;//变为新关键字的目标位置
        if (target == left_num) {//新键刚好可以放中间
            //拷贝关键字
            //拷贝孩子节点
            new_keys[0] = key;
            new_data[0] = value;
            for (int idx = 0; idx < this->get_size_()-left_num; idx++) {
                new_keys[1+idx] = this->keys[left_num + idx];
                new_data[1+idx] = this->data[left_num + idx];
            }
            //调整元素数量
            this->set_size_(left_num);
            new_node->set_size_(right_num);



        } else if (target < left_num) {//新键放在分裂后的左节点
            //拷贝关键字
            //拷贝孩子节点
            for (int idx = 0; idx < right_num; idx++) {
                new_keys[idx] = this->keys[left_num -1 + idx];
                new_data[idx] = this->data[left_num -1 + idx];
            }

            //调整节点关键字数量
            this->set_size_(left_num);
            new_node->set_size_(right_num);

            //左树空出关键字和数据插槽
            for (int idx = this->get_size_(); idx > target; idx--) {
                this->keys[idx] = this->keys[idx - 1];
                this->data[idx] = this->data[idx-1];
            }

            this->keys[target] = key;
            this->data[target] = value;

        } else {//新键放在分裂后的右节点
            for (int idx = 0; idx < target - left_num; idx++) {
                new_keys[idx] = this->keys[left_num + idx];
                new_data[idx] = this->data[left_num + idx];
            }

            //target在新的数组中的索引为target - left_num-1;
            new_keys[target - left_num] = key;
            new_data[target - left_num] = value;

            //继续拷贝剩下的
            for (int idx = target - left_num+1; idx < right_num; idx++) {
                new_keys[idx] = this->keys[left_num + idx-1];
                new_data[idx] = this->data[left_num + idx-1];
            }

            //调整节点元素个数
            this->set_size_(left_num);
            new_node->set_size_(right_num);
        }
        new_node->next = this->next;
        if(this->next != nullptr){
            this->next->prev = new_node;
        }
        new_node->prev = shared_from_this();
        this->next = new_node;

        return insert_result{new_keys[0], shared_from_this(), new_node};
    }
}

/**
 * 非叶节点从左兄弟借取节点
 * @param parent
 * @param pos
 * @param lsibling
 */
void InternalNode::borrow_from_left_sibling(shared_ptr<BaseNode> parent, int pos, shared_ptr<BaseNode> lsibling) {
    //本节点空出关键字以及指针位置
    //父节点关键字下降
    //左兄弟最右关键字上升
    //左兄弟最右关键字的右边kid也需要转移
    //调整元素个数

    auto lsibling_real = dynamic_pointer_cast<InternalNode>(lsibling);

    for(int idx = this->get_size_();idx>0;idx--){
        this->keys[idx] = this->keys[idx-1];
    }
    for(int idx = this->get_size_()+1;idx>0;idx--){
        this->children[idx] = this->children[idx-1];
    }

    auto& pkeys = parent->get_keys();
    this->keys[0] = pkeys[pos];

    auto& lkeys = lsibling_real->get_keys();
    pkeys[pos] = lkeys[lsibling_real->get_size_()-1];

    auto& pointers = lsibling_real->get_pointers();

    this->children[0] = pointers[lsibling_real->get_size_()];

    //调整元素个数
    this->incr();//
    lsibling->decl();
}

/**
 *
 * @param parent
 * @param pos
 * @param rsibling
 */
void InternalNode::borrow_from_right_sibling(shared_ptr<BaseNode> parent, int pos, shared_ptr<BaseNode> rsibling) {
    //父节点关键字下降
    //右兄弟最左关键字上升
    //右兄弟最左关键字的左边kid也需要转移
    //右兄弟的数组紧凑一波
    //调整元素个数

    auto rsibling_real = dynamic_pointer_cast<InternalNode>(rsibling);

    auto& pkeys = parent->get_keys();
    this->keys[this->get_size_()] = pkeys[pos];

    auto& rkeys = rsibling_real->get_keys();
    pkeys[pos] = rkeys[0];

    auto& pointers = rsibling_real->get_pointers();

    this->children[this->get_size_()+1] = pointers[0];

    //是紧凑右子树，不是紧凑左子树
    for(int idx = 0;idx<rsibling_real->get_size_()-1;idx++){
        rkeys[idx] = rkeys[idx+1];//TODO
    }
    for(int idx = 0;idx<=rsibling_real->get_size_()-1;idx++){
        pointers[idx] = pointers[idx+1];
    }

    //调整元素个数
    this->incr();//
    rsibling->decl();
}

/**
 * 合并两颗子树
 * @param parent
 * @param pos
 * @param other
 */
void InternalNode::merge(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> other) {
    //0父节点关键字下降
    //1移动关键字
    //2移动指针
    //3修改本对象的关键字数量
    //4紧凑父节点
    //  [0][1]        [0][1]
    // [0][1][2]     [0][1][2]
    auto other_raw = dynamic_pointer_cast<InternalNode>(other);
    auto num1 = this->get_size_();
    auto num2 = other_raw->get_size_();
    this->keys[this->get_size_()] = parent->get_keys()[pos];

    auto& keys = other_raw->get_keys();
    for(int idx = 0;idx<num2;idx++){
        this->keys[num1+1+idx] = keys[idx];
    }

    auto& pointers = other_raw->get_pointers();

    for(int idx=0;idx<=num2;idx++){//
        this->children[num1+1+idx] = pointers[idx];
    }

    this->set_size_(num1+num2+1);

    //紧凑父节点
    //移动key以及pointers
    //modify size
    auto parent_real = dynamic_pointer_cast<InternalNode>(parent);
    auto& pkeys = parent_real->get_keys();
    auto& ppointers = parent_real->get_pointers();
    for(int idx = pos;idx<parent_real->get_size_()-1;idx++){
        pkeys[idx] = pkeys[idx+1];
    }
    for(int idx=pos+1;idx<parent_real->get_size_();idx++){
        ppointers[idx] = ppointers[idx+1];
    }
    parent->decl();
}

/**
 * 非叶节点删除关键字
 * 思路：递归删除之后检查本节点是否失去平衡
 * 若失衡：
 * 1检查是否可借用
 * 2检查是否可合并
 * @param root
 * @param key
 * @return
 */
bool InternalNode::remove(shared_ptr<BaseNode> root, int key){
    auto target = index_(this->keys,key,0, this->get_size_()-1);

    auto res = this->children[target+1]->remove(this->children[target+1],key);

    //检查是否失去balance
    if(this->children[target+1]->get_size_()<(BPlusTree::DEGREE-1)/2){
        auto left_sibling = target>-1? this->children[target]: nullptr;
        auto right_sibling = target < this->get_size_()-1? this->children[target+2]: nullptr;

        if(left_sibling != nullptr && left_sibling->get_size_()>(BPlusTree::DEGREE-1)/2){
            this->children[target+1]->borrow_from_left_sibling(root,target,left_sibling);
        }else if(right_sibling != nullptr && right_sibling->get_size_()>(BPlusTree::DEGREE-1)/2){
            this->children[target+1]->borrow_from_right_sibling(root,target+1,right_sibling);
        }else if(left_sibling != nullptr){//与左兄弟合并
            left_sibling->merge(root,target,this->children[target+1]);
        }else{//与右兄弟合并
            this->children[target+1]->merge(root,target+1,right_sibling);
        }
    }
    return res;
}

/**
 * 叶子节点左兄弟借用
 * @param parent
 * @param pos
 * @param lsibling
 */
void LeafNode::borrow_from_left_sibling(shared_ptr<BaseNode> parent, int pos, shared_ptr<BaseNode> lsibling) {
    //本节点空出关键字以及数据位置
    //左兄弟最右关键字上升
    //左兄弟最右关键字的右边kid也需要转移
    //调整元素个数

    auto lsibling_real = dynamic_pointer_cast<LeafNode>(lsibling);

    for(int idx = this->get_size_();idx>0;idx--){
        this->keys[idx] = this->keys[idx-1];
    }
    for(int idx = this->get_size_();idx>0;idx--){
        this->data[idx] = this->data[idx-1];
    }

    auto& lkeys = lsibling_real->get_keys();
    this->keys[0] = lkeys[lsibling_real->get_size_()-1];

    auto& ldata = lsibling_real->get_data();
    this->data[0] = ldata[lsibling_real->get_size_()-1];

    auto& pkeys = parent->get_keys();
    pkeys[pos] = this->keys[0];

    //调整元素个数
    this->incr();//
    lsibling->decl();
}

/**
 * 叶子节点右兄弟借用
 * @param parent
 * @param pos
 * @param rsibling
 */
void LeafNode::borrow_from_right_sibling(shared_ptr<BaseNode> parent, int pos, shared_ptr<BaseNode> rsibling) {
    //本节点空出关键字以及指针位置
    //左兄弟最右关键字上升
    //左兄弟最右关键字的右边kid也需要转移
    //调整元素个数

    auto rsibling_real = dynamic_pointer_cast<LeafNode>(rsibling);

    auto& rkeys = rsibling_real->get_keys();
    this->keys[this->get_size_()] = rkeys[0];

    auto& rdata = rsibling_real->get_data();
    this->data[this->get_size_()] = rdata[0];

    for(int idx = 0;idx<rsibling_real->get_size_()-1;idx++){
        rkeys[idx] = rkeys[idx+1];
        rdata[idx] = rdata[idx+1];
    }

    auto& pkeys = parent->get_keys();
    pkeys[pos] = rkeys[0];

    //调整元素个数
    this->incr();//
    rsibling->decl();
}

/**
 * 叶子节点的合并
 * @param parent
 * @param pos
 * @param other
 */
void LeafNode::merge(shared_ptr<BaseNode> parent,int pos,shared_ptr<BaseNode> other) {
    //1 移动keys
    //2 移动数据
    //3 调整当前元素数量
    //4 调整双向链表指针

    auto other_raw = dynamic_pointer_cast<LeafNode>(other);
    auto num1 = this->get_size_();
    auto num2 = other_raw->get_size_();
    //this->keys[this->get_size_()] = parent->get_keys()[pos];

    auto& keys = other_raw->get_keys();
    for(int idx = 0;idx<num2;idx++){
        this->keys[num1+idx] = keys[idx];
    }

    auto& data = other_raw->get_data();
    for(int idx=0;idx<num2;idx++){
        this->data[num1+idx] = data[idx];
    }

    this->set_size_(num1+num2);

    //维护双向链表
    if(other_raw->next != nullptr){
        other_raw->next->prev = shared_from_this();
        this->next = other_raw->next;
    }
    other_raw->prev = nullptr;
    other_raw->next = nullptr;

    //紧凑父节点
    //移动数据以及指针
    auto parent_real = dynamic_pointer_cast<InternalNode>(parent);
    auto& pkeys = parent_real->get_keys();
    auto& ppointers = parent_real->get_pointers();
    for(int idx = pos;idx<parent_real->get_size_()-1;idx++){
        pkeys[idx] = pkeys[idx+1];
    }
    for(int idx = pos+1;idx<parent_real->get_size_();idx++){
        ppointers[idx] = ppointers[idx+1];
    }
    parent_real->decl();
}

/**
 * 叶子节点删除
 * @param root
 * @param key
 * @return
 */
bool LeafNode::remove(shared_ptr<BaseNode> root, int key){
    auto target = index_(this->keys,key,0, this->get_size_()-1);

    if(target>-1 && this->keys[target]==key){
        //紧凑
        for(int idx = target;idx<this->get_size_()-1;idx++){
            this->keys[idx] = this->keys[idx+1];
            this->data[idx] = this->data[idx+1];
        }
        this->decl();
        return true;
    }else{
        cout<<"ERROR:[Remove] target key does not exist."<<key<<endl;
        return false;
    }
}

/**
 * 插入接口s
 * @param key
 * @param value
 */
void BPlusTree::insert(int key, int value) {
    if(this->root == nullptr){
        return;
    }
    auto res = this->root->insert(this->root,key,value);
    if(res.right != nullptr){//root node
        auto new_root = make_shared<InternalNode>(BPlusTree::DEGREE-1);
        new_root->get_keys()[0] = res.middle_key;
        new_root->get_pointers()[0] = res.left;
        new_root->get_pointers()[1] = res.right;
        new_root->incr();
        this->root = new_root;
    }
}


/**
 * 删除B+树的关键字
 * B+树的删除情况比B树少一种
 * @param key
 */
void BPlusTree::remove(int key){//为什么有必要进行特殊处理？因为需要修改根节点指针
    this->root->remove(this->root,key);
    //检查是否需要更换根节点
    if(!this->root->is_leaf() && this->root->get_size_()==0){
        auto root_real = dynamic_pointer_cast<InternalNode>(this->root);
        this->root = root_real->get_pointers()[0];
    }
}

void BPlusTree::query(int key) {
    shared_ptr<BaseNode> cur = this->root;
    int target = -1;
    while(!cur->is_leaf()) {//不是叶节点就一直找
        target = index_(cur->get_keys(), key, 0, cur->get_size_() - 1);
        cur = dynamic_pointer_cast<InternalNode>(cur)->get_pointers()[target + 1];
    }
    shared_ptr<LeafNode> leaf = dynamic_pointer_cast<LeafNode>(cur);
    target = index_(cur->get_keys(),key,0,cur->get_size_()-1);
    if(target>-1 && leaf->get_keys()[target]==key){
        cout<<"query key "<<key<<":"<<leaf->get_data()[target]<<endl;
    }else{
        cout<<"ERROR:[Query] key not exist."<<endl;
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
    cout<<"Level print:"<<endl;
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

//利用叶子节点的双向链表打印
void BPlusTree::linear_print(){
    shared_ptr<BaseNode> cur = this->root;
    while(!cur->is_leaf()){//到达最底层
        auto temp = dynamic_pointer_cast<InternalNode>(cur);
        cur = temp->get_pointers()[0];
    }
    shared_ptr<LeafNode> p = dynamic_pointer_cast<LeafNode>(cur);
    cout<<"Linear print:"<<endl;
    while(p!= nullptr){
        cout<<"[";
        for(int idx=0;idx<p->get_size_();idx++){
            cout<< p->get_keys()[idx]<<" ";
        }
        cout<<"]";
        p = p->next;
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

//不要怕recursive程序

/**
 * 用于基准测试
 */
void benchmark(){
    BPlusTree bPlusTree(10);//最多四个关键字
    for(int idx=1;idx<1000;idx++){
        bPlusTree.insert(idx,idx);
    }
//    bPlusTree.level_print();
    bPlusTree.linear_print();
    for(int idx=1;idx<800;idx++){
        bPlusTree.remove(idx);
    }
    bPlusTree.level_print();
//    bPlusTree.linear_print();
    for(int idx=750;idx<950;idx++){
        bPlusTree.query(idx);
    }

}

void debug(){
    //测试插入分裂
    BPlusTree bPlusTree(5);//最多四个关键字
    bPlusTree.insert(100,10);
    bPlusTree.insert(101,10);
    bPlusTree.insert(102,10);
    bPlusTree.insert(103,10);
    bPlusTree.insert(104,10);
    bPlusTree.insert(105,10);
    bPlusTree.insert(106,10);
    bPlusTree.insert(107,10);
    bPlusTree.insert(108,10);
    bPlusTree.insert(109,10);
    bPlusTree.insert(110,10);
    bPlusTree.insert(111,10);
    bPlusTree.insert(112,10);

    bPlusTree.level_print();
    //测试删除合并借用
    bPlusTree.remove(104);
    bPlusTree.level_print();
    bPlusTree.remove(103);
    bPlusTree.level_print();
    bPlusTree.remove(108);
    bPlusTree.level_print();
    bPlusTree.remove(109);
    bPlusTree.level_print();
    bPlusTree.linear_print();
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    benchmark();
//    debug();
    return 0;
}