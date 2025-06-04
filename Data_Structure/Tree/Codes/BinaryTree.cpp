#include <iostream>
using namespace std;


// Definition for a binary tree
template <class elemType> // 二叉树的元素类型
class binaryTree{
public:
    virtual void clear() = 0; // 清空
    virtual bool empty() const = 0; // 判空
    virtual int height() const = 0; // 树的高度
    virtual int size() const = 0; // 树的节点数
    virtual void preOrderTraverse() const = 0; // 前序遍历
    virtual void inOrderTraverse() const = 0; // 中序遍历
    virtual void postOrderTraverse() const = 0; // 后序遍历
    virtual void levelOrderTraverse() const = 0; // 层次遍历
    virtual ~binaryTree() {}; // 虚析构函数
};

template <class elemType>
class BinaryLinkList:public binaryTree<elemType>{
    struct Node{
        Node *left, *right; // 左右孩子指针
        elemType data; // 节点数据
        Node():left(nullptr), right(nullptr){} // 默认构造函数
        Node(elemType value, Node *l = nullptr, Node *r = nullptr){
            data = value;
            left = l;
            right = r;
        } // 带参数的构造函数
        ~Node(){}
    };
    Node* root;
    void clear(Node* t); // 私有，清空
    int size(Node* t) const; // 私有，二叉树的结点总数
    int height(Node* t) const; // 私有，二叉树的高度
    int leafNum(Node* t) const; // 私有，二叉树的叶结点数
    void preOrder(Node* t) const; // 私有，递归前序遍历
    void inOrder(Node* t) const; // 私有，递归中序遍历
    void postOrder(Node* t) const; // 私有，递归后序遍历
    void preOrderCreate(elemType flag, Node* &t); // 私有，创建二叉树
public:
    BinaryLinkList():root(nullptr){} // 构造空二叉树
    ~BinaryLinkList(){clear();} // 析构函数，清空二叉树
    bool empty() const {return root == nullptr;} // 判空
    void clear() {if(root) clear(root); root = nullptr; } // 清空二叉树
    int size() const {return size(root);} // 返回二叉树的结点总数
    int height() const {return height(root);} // 返回二叉树的高度
    void preOrderTraverse() const {if(root) preOrder(root);} // 前序遍历
    void inOrderTraverse() const {if(root) inOrder(root);} // 中序遍历
    void postOrderTraverse() const {if(root) postOrder(root);} // 后序遍历
    void levelOrderTraverse() const; // 层次遍历
    void preOrderWithStack() const; // 非递归前序遍历
    void inOrderWithStack() const; // 非递归中序遍历
    void postOrderWithStack() const; // 非递归后序遍历
    void levelOrderCreate(elemType flag); // 利用带外部结点的层次序列创建二叉树
    void preOrderCreate(elemType flag){
        preOrderCreate(flag, root); // 利用带外部结点的前序序列创建二叉树
    }
};

template <class elemType>
void BinaryLinkList<elemType>::preOrder(Node* t) const {
    if (t){
        cout << t->data << " "; // 访问根结点
        inOrder(t->left); // 访问左子树
        inOrder(t->right); // 访问右子树
    }
}

template <class elemType>
void BinaryLinkList<elemType>::inOrder(Node* t) const {
    if (t){
        inOrder(t->left); // 访问左子树
        cout << t->data << " "; // 访问根结点
        inOrder(t->right); // 访问右子树
    }
}

template <class elemType>
void BinaryLinkList<elemType>::postOrder(Node* t) const {
    if (t){
        postOrder(t->left); // 访问左子树
        postOrder(t->right); // 访问右子树
        cout << t->data << " "; // 访问根结点
    }
}

template <class elemType>
void BinaryLinkList<elemType>::preOrderWithStack() const {
    stack<Node*> s; // 创建一个栈
    Node* p = root; // 工作指针
    while(!s.empty() || p){ // 栈非空或者p非空
        if(p){
            cout << p->data << " "; // 访问根结点
            s.push(p); // 将根结点入栈
            p = p->left; // 访问左子树
        } else {
            p = s.top(); // 栈顶元素
            s.pop(); // 弹出栈顶元素
            p = p->right; // 访问右子树
        }
    }
}

template <class elemType>
void BinaryLinkList<elemType>::inOrderWithStack() const {
    stack<Node*> s; // 创建一个栈
    Node* p = root; // 工作指针
    while(!s.empty() || p){ // 栈非空或者p非空
        if(p){
            s.push(p); // 将根结点入栈
            p = p->left; // 访问左子树
        } else {
            p = s.top(); // 栈顶元素
            s.pop(); // 弹出栈顶元素
            cout << p->data << " "; // 访问根结点
            p = p->right; // 访问右子树
        }
    }
}

template <class elemType>
void BinaryLinkList<elemType>::postOrderWithStack() const{
    enum ChildType{Left, Right}; // 定义子树类型
    struct StackElem{
        Node* pointer;
        ChildType flag;
    };
    StackElem elem;
    stack<StackElem> s; // 创建一个栈
    Node* p = root; // 工作指针
    while(!s.empty() || p){ // 栈非空或者p非空
        while (p != nullptr){
            elem.pointer = p;
            elem.flag = Left; // 标记为左子树
            s.push(elem);
            p = p->left;
        }
        elem = s.top();
        s.pop(); // 弹出栈顶元素
        p = elem.pointer;
        if (elem.flat == Left){ // 从左边回来，已经遍历完左子树
            elem.flag = Right;
            s.push(elem);
            p = p->right;
        }else{
            cout << p->data << " "; // 访问根结点
            p = nullptr;
        }
    }
}

template <class elemType>
void BinaryLinkList<elemType>::levelOrderTraverse() const {
    queue<Node*> que; // 创建一个队列
    Node* p = root;
    if(p) que.push(p); // 根结点入队
    while (!que.empty()){ // 队列非空
        p = que.front(); // 队首元素
        que.pop(); // 出队
        cout << p->data << " "; // 访问根结点
        if (p->left) que.push(p->left); // 左子树入队
        if (p->right) que.push(p->right); // 右子树入队
    }
}

template <class elemType>
void BinaryLinkList<elemType>::preOrderCreate(elemType flag, Node* &t){
    elemType value;
    cin >> value; // 输入结点值
    if (value == flag) { // 如果是外部结点
        t = nullptr; // 置空
    } else {
        t = new Node(value); // 创建新结点
        preOrderCreate(flag, t->left); // 创建左子树
        preOrderCreate(flag, t->right); // 创建右子树
    }
}

template <class elemType>
int BinaryLinkList<elemType>::size(Node* t) const {
    if (t == nullptr) return 0; // 空树
    return 1 + size(t->left) + size(t->right); // 递归计算
}

template <class elemType>
int BinaryLinkList<elemType>::height(Node* t) const {
    if (t == nullptr) return 0; // 空树
    int lh = height(t->left); // 左子树高度
    int rh = height(t->right); // 右子树高度
    return ((lh > rh) ? lh:rh) + 1; // 返回最大高度
}

template <class elemType>
int BinaryLinkList<elemType>::leafNum(Node* t) const {
    if (t == nullptr) return 0; // 空树
    if (t->left == nullptr && t->right == nullptr) return 1; // 叶结点
    return leafNum(t->left) + leafNum(t->right); // 递归计算
}

template <class elemType>
void BinaryLinkList<elemType>::clear(Node* t){
    if (t->left) clear(t->left); // 清空左子树
    if (t->right) clear(t->right); // 清空右子树
    delete t; // 删除当前结点
}

int main(){
    cout << "=== Binary Tree Test Suite ===" << endl;

    // 创建一个二叉树
    BinaryLinkList<int> tree;
    cout << "Creating binary tree with external node marker 0:" << endl;
    tree.preOrderCreate(-1); // 从前序序列创建二叉树

    // 遍历二叉树
    cout << "\nPre-order Traversal: ";
    tree.preOrderTraverse();
    cout << "\nIn-order Traversal: ";
    tree.inOrderTraverse();
    cout << "\nPost-order Traversal: ";
    tree.postOrderTraverse();
    cout << "\nLevel-order Traversal: ";
    tree.levelOrderTraverse();

    // 清空二叉树
    tree.clear();
    cout << "\nBinary tree cleared." << endl;

    return 0;
}