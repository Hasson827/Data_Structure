#include <iostream>
using namespace std;


// Definition for a binary tree
template <class elemType> // 二叉树的元素类型
class Tree{
public:
    virtual void clear() = 0; // 清空
    virtual bool empty() const = 0; // 判空
    virtual int height() const = 0; // 树的高度
    virtual int size() const = 0; // 树的节点数
    virtual void preOrderTraverse() const = 0; // 前序遍历
    virtual void inOrderTraverse() const = 0; // 中序遍历
    virtual void postOrderTraverse() const = 0; // 后序遍历
    virtual void levelOrderTraverse() const = 0; // 层次遍历
    virtual ~Tree() {}; // 虚析构函数
};