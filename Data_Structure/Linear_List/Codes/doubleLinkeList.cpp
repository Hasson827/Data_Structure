#include "List.h"
#include <iostream>
using namespace std;

#include "List.h"
template <class elemType>
class doubleLinkList : public List<elemType>{
private:
    struct Node{
        elemType data; // 数据域
        Node *prior, *next; // 指针域，两个指针分别指向前驱和后继
        Node(const elemType value, Node* p = nullptr, Node* n = nullptr){
            data = value;
            prior = p;
            next = n;
        }
        Node():next(nullptr), prior(nullptr){}
        ~Node(){}
    };
    Node *head, *tail; // 头尾指针
    int curLength; // 双链表的当前长度
    Node* getPosition(int i)const; // 返回指向位序为i处结点的指针
public:
    doubleLinkList();
    ~doubleLinkList();
    void clear(); // 清空双链表，使之成为空表
    bool empty() const{return head->next == tail;} // 判空
    int size() const{return curLength;} // 求双链表的长度
    void insert(int i, const elemType& value); // 在位序i处插入值为value的结点，表长+1
    void remove(int i); // 删除位序i处的结点，表长-1
    int search(const elemType& value) const; // 查找值为value的结点的位序
    elemType visit(int i) const; // 访问位序为i的结点的值
    void traverse() const; // 遍历双链表
    void inverse(); // 逆置双链表
};

template <class elemType>
doubleLinkList<elemType>::doubleLinkList(){
    head = new Node; // 头指针指向头结点
    tail = new Node; // 尾指针指向尾部头结点
    head->next = tail;
    tail->prior = head;
    curLength = 0;
}

template <class elemType>
doubleLinkList<elemType>::~doubleLinkList(){
    clear();
    delete head;
    delete tail;
}

template <class elemType>
void doubleLinkList<elemType>::clear(){
    // 清空操作时不再考虑结点的前驱域是否断链
    Node *p = head->next, *tmp;
    head->next = tail; // 头结点的后继时尾部头结点
    tail->prior = head; // 尾部头结点的前驱时头结点
    while (p != tail){
        tmp = p->next;
        delete p;
        p = tmp;
    }
    curLength = 0;
}

template <class elemType>
typename doubleLinkList<elemType>::Node *doubleLinkList<elemType>::getPosition(int i) const{
    // 位序i的合法范围是[-1,curLength]，若i==-1，则定位到头结点，若i==curLength，则定位到tail指向的尾部头结点
    Node *p = head;
    int count = 0;
    if (i < -1 || i > curLength) return nullptr; // 当i非法时返回nullptr
    while (count <= i){
        p = p->next;
        count++;
    }
    return p; // 返回指向位序为i的结点的指针
}

template <class elemType>
int doubleLinkList<elemType>::search(const elemType& value) const{
    Node *p = head->next;
    int i = 0;
    while (p != tail && p->data != value){
        p = p->next;
        i++;
    }
    if (p == tail) return -1;
    else return i;
}

template <class elemType>
void doubleLinkList<elemType>::insert(int i, const elemType& value){
    Node *p, *tmp;
    if (i < 0 || i > curLength) // 合法的插入范围为[0,curLength]
        throw outOfRange(); // 插入位置非法，抛出异常
    p = getPosition(i); // 若i==curLength，则定位到tail指向的结点
    tmp = new Node(value, p->prior, p); // tmp节点插在p节点之前
    p->prior->next = tmp; // p原先的前驱的后继指向tmp
    p->prior = tmp; // 修改p的前驱为tmp
    ++curLength;
}

template <class elemType>
void doubleLinkList<elemType>::remove(int i){
    Node *p;
    if (i < 0 || i > curLength-1) // 合法的删除范围为[0,curLength-1]
        throw outOfRange(); // 当待删结点不存在时，抛出异常
    p = getPosition(i);
    p->prior->next = p->next;
    p->next->prior = p->prior;
    delete p;
    --curLength;
}

template <class elemType>
elemType doubleLinkList<elemType>::visit(int i) const{
    // visit不能直接用getPosition判断范围是否合法，因为其定位范围为[-1,cueLength]
    if (i < 0 || i > curLength-1) // 合法的访问范围为[0,cueLength-1]
        throw outOfRange(); // 当节点不存在时，抛出异常
    Node *p = getPosition(i);
    return p->data;
}

template <class elemType>
void doubleLinkList<elemType>::traverse() const{
    Node *p = head->next;
    while (p != tail){
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}

template <class elemType>
void doubleLinkList<elemType>::inverse(){
    Node *tmp, *p = head->next; // p是工作指针，指向首元结点
    head->next = tail; // 构成空双链表
    tail->prior = head;
    while (p != tail){
        tmp = p->next; // 保存p的后继
        p->next = head->next;
        p->prior = head; // p结点插到头结点的后面
        head->next->prior = p; 
        head->next = p;
        p = tmp;
    }
}