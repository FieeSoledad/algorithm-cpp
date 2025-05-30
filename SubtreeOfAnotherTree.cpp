
#include<iostream>
using namespace std;
//Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if(root==nullptr && subRoot==nullptr){
            return true;
        }else if(root!=nullptr&&subRoot==nullptr ){
            return true;
        }else if(root==nullptr && subRoot!=nullptr){
            return false;
        }
        return same(root,subRoot) || isSubtree(root->left,subRoot) || isSubtree(root->right,subRoot);
    }

    bool same(TreeNode* root,TreeNode* subroot){
        if(root==nullptr && subroot==nullptr){
            return true;
        }
        if(root!=nullptr && subroot!=nullptr){
            return root->val==subroot->val && same(root->left,subroot->left) && same(root->right,subroot->right);
        }
        return false;
    }
};

//判断某个树是否是另一颗树的子树，如何使用递归完成
//可以使用字符串的序列化，使用null标识空节点
int main(){
    return 0;
}
