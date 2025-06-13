#include <iostream>
#include <queue> // STL队列
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
    virtual void postOrderTraverse() const = 0; // 后序遍历
    virtual void levelOrderTraverse() const = 0; // 层次遍历
    virtual ~Tree() {}; // 虚析构函数
};

template <class elemType>
class childSiblingTree : public Tree<elemType> {
    struct Node{
        elemType data; // 节点数据
        Node* firstChild; // 指向第一个子节点
        Node* nextSibling; // 指向下一个兄弟节点
        Node(const elemType& value) : data(value), firstChild(nullptr), nextSibling(nullptr) {}
        Node() : firstChild(nullptr), nextSibling(nullptr) {}
        ~Node(){}
    };
    Node* root; // 根节点
    void clear(Node* t); // 清空子树
    int size(Node* t) const; // 计算子树的节点数
    int height(Node* t) const; // 计算子树的高度
    int leafNum(Node* t) const; // 计算子树的叶子节点数
    void preOrder_1(Node* t) const; // 前序遍历（递归）
    void preOrder_2(Node* t) const; // 前序遍历（非递归）
    void postOrder_1(Node* t) const; // 后序遍历（递归）
    void postOrder_2(Node* t) const; // 后序遍历（非递归）
    void preOrderCreate(elemType flag, Node* &t); // 前序创建树
public:
    childSiblingTree(): root(nullptr) {} // 默认构造函数
    ~childSiblingTree() { clear(); } // 析构函数
    void clear(){if (root) clear(root); root = nullptr; } // 清空树
    bool empty() const { return root == nullptr; } // 判空
    int height() const { return height(root); } // 树的高度
    int size() const { return size(root); } // 树的节点数
    void preOrderTraverse() const {if(root) preOrder_1(root); } // 前序遍历（递归）
    void postOrderTraverse() const {if(root) postOrder_1(root); } // 后序遍历（递归）
    void levelOrderTraverse() const; // 层次遍历
    void preOrderCreate(elemType flag){ // 前序创建树
        preOrderCreate(flag, root);
    }
};

template <class elemType>
void childSiblingTree<elemType>::preOrder_1(Node* t) const {
    if (t != nullptr) {
        cout << t->data << " "; // 访问当前节点
        preOrder_1(t->firstChild); // 递归访问第一个子节点
        t = t->nextSibling; // 移动到下一个兄弟节点
    }
}

template <class elemType>
void childSiblingTree<elemType>::postOrder_1(Node* t) const {
    if (t != nullptr) {
        postOrder_1(t->firstChild); // 递归访问第一个子节点
        cout << t->data << " "; // 访问当前节点
        t = t->nextSibling; // 移动到下一个兄弟节点
    }
}

template <class elemType>
void childSiblingTree<elemType>::preOrder_2(Node* t) const {
    if (t != nullptr){
        cout << t->data << " "; // 访问当前节点
        preOrder_2(t->firstChild); // 递归访问第一个子节点
        preOrder_2(t->nextSibling); // 递归访问下一个兄弟节点
    }
}

template <class elemType>
void childSiblingTree<elemType>::postOrder_2(Node* t) const {
    if (t != nullptr){
        postOrder_2(t->firstChild); // 递归访问第一个子节点
        cout << t->data << " "; // 访问当前节点
        postOrder_2(t->nextSibling); // 递归访问下一个兄弟节点
    }
}

template <class elemType>
void childSiblingTree<elemType>::levelOrderTraverse() const {
    queue<Node*> Q; // STL队列
    Node* p = root; // 工作指针
    while (p != nullptr){
        Q.push(p); // 将当前节点入队
        p = p->nextSibling; // 指向下一个兄弟节点
    }
    while (!Q.empty()){
        p = Q.front(); // 获取队首元素
        Q.pop(); // 出队
        cout << p->data << " "; // 访问当前节点
        p = p->firstChild; // 指向第一个子节点
        while (p != nullptr) {
            Q.push(p); // 将子节点入队
            p = p->nextSibling; // 指向下一个兄弟节点
        }
    }
}

template <class elemType>
void childSiblingTree<elemType>::clear(Node* t) {
    if (t->firstChild) clear(t->firstChild); // 清空第一个子节点
    if (t->nextSibling) clear(t->nextSibling); // 清空下一个兄弟节点
    delete t; // 删除当前节点
}

template <class elemType>
int childSiblingTree<elemType>::size(Node* t) const {
    if (t == nullptr) return 0; // 空节点返回0
    return 1 + size(t->firstChild) + size(t->nextSibling); // 当前节点+子节点+兄弟节点
}

template <class elemType>
int childSiblingTree<elemType>::leafNum(Node* t) const {
    if (t == nullptr) return 0; // 空节点返回0
    if (t->firstChild == nullptr) return 1 + leafNum(t->nextSibling); // 如果没有子节点，则是叶子节点
    return leafNum(t->firstChild) + leafNum(t->nextSibling); // 递归计算子节点和兄弟节点的叶子节点数
}

template <class elemType>
int childSiblingTree<elemType>::height(Node* t) const {
    if (t == nullptr) return 0; // 空节点高度为0
    else{
        int lh = height(t->firstChild), rh = height(t->nextSibling); // 计算子节点和兄弟节点的高度
        return (1+lh > rh ? 1+lh : rh); // 返回较大高度
    }
}

template <class elemType>
void childSiblingTree<elemType>::preOrderCreate(elemType flag, Node* &t) {
    elemType value;
    cin >> value; // 输入节点值
    if(value != flag){
        t = new Node(value); // 创建新节点
        preOrderCreate(flag, t->firstChild); // 递归创建第一个子节点
        preOrderCreate(flag, t->nextSibling); // 递归创建下一个兄弟节点
    } else {
        t = nullptr; // 如果输入值为标志值，则设置为nullptr
    }
}

int main(){
    childSiblingTree<int> tree; // 创建树对象
    cout << "请输入前序遍历的节点值，以-1结束：" << endl;
    tree.preOrderCreate(-1); // 前序创建树，-1为结束标志
    cout << "前序遍历结果：" << endl;
    tree.preOrderTraverse(); // 前序遍历
    cout << endl;

    cout << "后序遍历结果：" << endl;
    tree.postOrderTraverse(); // 后序遍历
    cout << endl;

    cout << "层次遍历结果：" << endl;
    tree.levelOrderTraverse(); // 层次遍历
    cout << endl;

    cout << "树的高度：" << tree.height() << endl; // 树的高度
    cout << "树的节点数：" << tree.size() << endl; // 树的节点数
    return 0;
}