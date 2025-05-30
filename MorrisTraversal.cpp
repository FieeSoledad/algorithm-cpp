//
// Created by hsl on 2024/11/16.
//


//空间复杂度为O1，时间复杂度为O(n)的Morris遍历


//Morris序如何加工成三序遍历？
//先序：中左右    如何改正呢第一次
//中序：只到一次的直接打印，到达两次的第二次打印

#include<iostream>
using namespace std;
#include<vector>


//Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

//先序遍历
vector<int> preorder_travel(TreeNode* root){
    vector<int> ans;
    //Morris序
    TreeNode* cur = root;
    TreeNode* mostRight = nullptr;
    while(cur!=nullptr){
        mostRight = cur->left;
        if(mostRight!=nullptr){
            while(mostRight->right!=nullptr && mostRight->right!=cur){
                mostRight = mostRight->right;
            }
            if(mostRight->right==nullptr){//第一次到达
                ans.push_back(cur->val);
                mostRight->right = cur;
                cur = cur->left;
                //这里不需要再直接向右
                continue;
            }else{//第二次到达
                mostRight->right = nullptr;
            }
        }else{
            ans.push_back(cur->val);
        }
        cur =  cur->right;
    }
    return ans;
}


//中序遍历
//左中右 没有左树的节点是直接收集，没有左就是轮到中了，否则是第二次收集节点
//vector<int> midorder_travel(TreeNode* root){
//
//}

//实现morris的后序遍历
//左右中的后序遍历
//思路：只在到达两次的节点处逆序收集左树的右边界
//逆序收集如何保证空间复杂度？使用翻转链表的方式
//vector<int> postorder_travel();

//Morris的问题是无法像DFS一样做强整合信息



//线索二叉树？

//Morris遍历能解决的问题是比较有限的，只能解决验证搜索二叉树是否有效这种简单场景
//中序遍历
bool is_valid_bst(TreeNode* root){
    bool ans = true;
    TreeNode* cur = root;
    TreeNode* mostRight = nullptr;
    TreeNode* prev = nullptr;

    while(cur!= nullptr) {
        mostRight = cur->left;
        if (mostRight != nullptr) {
            while(mostRight->right!= nullptr && mostRight->right!=cur){
                mostRight = mostRight->right;
            }
            if(mostRight->right== nullptr){
                mostRight->right = cur;
                cur = cur->left;
                continue;
            }else{
                mostRight->right = nullptr;
                if(prev!= nullptr && prev->val>=cur->val){
                    ans = false;
                }
                prev = cur;
            }
        } else {
            if (prev != nullptr && prev->val >= cur->val) {
                ans = false;
            }
            prev = cur;
        }
        cur = cur->right;
    }
    return ans;
}

//简单的求叶节点的最小深度
//每一层的高度确定，还有如何确定叶节点？（因为遍历过程中有更改树的结构，所以难以判断）
//除了整树的最右节点，每个叶节点都有右边的树部分

//Morris序遍历也不是不行诶
//cur移动时，维护前一个节点的深度
//这个告诉我们如何确定叶节点
int minDepth(TreeNode* root){
    int ans = INT_MAX;
    TreeNode* cur = root;
    TreeNode* mostRight = nullptr;
    int pre_level = 0;
    //Morris遍历到达的节点分为好多种
    while(cur!= nullptr){
        mostRight = cur->left;
        if(mostRight!= nullptr){
            while(mostRight->right!= nullptr && mostRight->right!=cur){
                mostRight = mostRight->right;
            }
            if(mostRight->right== nullptr){//第一次到达
                mostRight->right = cur;
                cur = cur->left;
                pre_level+=1;
                continue;
            }else{
                //第二次到达，恢复
                mostRight->right = nullptr;
                //说明是叶子节点
                if(mostRight->left== nullptr){
                    ans = min(ans,pre_level);
                }
                //维护高度，需要减去所谓左树右边界的长度等等
                int num = 1;
                TreeNode* temp = cur->left;
                while(temp->right!= nullptr){
                    temp = temp->right;
                    num+=1;
                }
                pre_level-=num;
            }
        }else{
            //当前节点没有左树，向右走
            pre_level+=1;
        }
        cur = cur->right;
    }
    TreeNode* temp = cur;
    int num = 0;
    while(temp->right!= nullptr){
        temp = temp->right;
        num+=1;
    }
    //只有叶子节点才参与评选
    if(temp->left== nullptr)
        ans = min(ans,1+num);
    return ans;
}

//Morris遍历比较难改的

int main(){

    return 0;
}