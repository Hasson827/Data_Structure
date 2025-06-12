#include "List.h"
#include <iostream>
using namespace std;

template <class elemType>
class linkList : public List<elemType>{
private:
    struct Node{ // 结点类型
        elemType data; // 结点的数据域
        Node* next; // 结点的指针域
        Node(const elemType value, Node* p = nullptr){ // 两个参数的构造函数
            data = value;
            next = p;
        }
        Node(Node* p = nullptr){ // 一个参数的构造函数
            next = p;
        }
    };
    Node* head; // 单链表的头指针
    Node* tail; // 单链表的尾指针
    int curLength; // 单链表的当前长度，牺牲空间换时间
    Node* getPosition(int i)const; // 返回指向位序为i的结点的指针
public:
    linkList(); // 构造函数
    ~linkList(); // 析构函数
    void clear(); // 将单链表清空，使其成为空表
    bool empty()const{return head->next == nullptr;} // 带头结点的单链表，判空
    int size()const{return curLength;} // 返回单链表的当前实际长度
    void insert(int i, const elemType& value); // 在位序i处插入值为value的结点，表长+1
    void remove(int i); // 删除位序i处的结点，表长-1
    int search(const elemType& value)const; // 查找值为value的结点第一次出现的位序
    int prior(const elemType& value)const; // 查找值为value的结点的前驱的位序
    elemType visit(int i)const; // 访问位序为i的结点的值，0定位到首元结点
    void traverse()const; // 遍历单链表
    void headCreate(); // "头插法"创建单链表
    void tailCreate(); // "尾插法"创建单链表
    void inverse(); // 逆置单链表
    linkList<elemType>* Union(linkList<elemType>* lb); // 合并两个有序单链表，结果存储在当前对象中
};

template <class elemType>
linkList<elemType>::linkList(){
    head = tail = new Node(); // 创建带有头节点的空表
    curLength = 0;
}

template <class elemType>
linkList<elemType>::~linkList(){
    clear(); // 清空单链表
    delete head; // 释放头结点
}

template <class elemType>
void linkList<elemType>::clear(){
    Node *p, *tmp; // p为工作指针，指向首元结点
    p = head->next; // 引入工作指针是为了防止随意修改头指针
    while(p != nullptr){
        tmp = p;
        p = p->next; // 指针后移
        delete tmp;
    }
    head->next = nullptr; // 头结点的指针域置空
    tail = head; // 头尾指针均指向头结点
    curLength = 0;
}

template <class elemType>
int linkList<elemType>::size()const{
    return curLength; // 直接返回curLength
}

template <class elemType>
void linkList<elemType>::traverse()const{
    Node* p = head->next; // 工作指针p指向首元结点
    while (p != nullptr){
        cout << p->data << " ";
        p = p->next; // 向后移动指针
    }
}

template <class elemType>
typename linkList<elemType>::Node* linkList<elemType>::getPosition(int i)const{
    if (i < -1 || i > curLength-1) // 合法查找范围为[-1,CurLength-1]
        return nullptr; // 当i非法时返回nullptr
    Node* p = head; // 工作指针p指向头结点
    int count = 0;
    while (count <= i){
        p = p->next;
        count++;
    }
    return p; // 返回指向位序为i的结点的指针
}

template <class elemType>
int linkList<elemType>::search(const elemType& value)const{
    Node* p = head->next; // 工作指针p指向首元结点
    int count = 0; // 首元结点的位序为0
    while (p != nullptr && p->data != value){
        p = p->next;
        count++;
    }
    if (p == nullptr)return -1; // 查找失败返回-1，这里-1并非头结点
    else return count; // 查找成功，count为结点的位序
}

template <class elemType>
int linkList<elemType>::prior(const elemType& value)const{
    Node* p = head->next; // p是工作指针，指向首元结点
    Node* pre = nullptr; // pre指向p的前驱
    int count = -1; // 注意：-1表示首元结点无前驱
    while(p && p->data != value){
        pre = p; // 前驱指针后移
        p = p->next; // 指向下个待处理结点
        count++;
    }
    if(p == nullptr) return -1; // 查找失败返回-1，这里-1并非头结点
    else return count; // 查找成功，count为结点的位序
}

template <class elemType>
void linkList<elemType>::insert(int i, const elemType& value){
    Node *p, *q;
    if (i < 0 || i > curLength) // 合法的插入范围为[0,curLength]
        throw outOfRange(); // 插入位置非法，抛出异常
    p = getPosition(i-1); // p是位序为i的结点的前驱
    q = new Node(value, p->next); // 申请新结点q，数据域为value，指针域为p->next
    p->next = question; // q结点插到p结点的后面
    if (p == tail) tail = q; // 若插入点在链表尾，则q成为新的尾节点
    curLength++;
}

template <class elemType>
void linkList<elemType>::remove(int i){
    Node *pre, *p; // p是待删结点，pre是其前驱
    if (i < 0 || i > curLength-1) // 合法的删除范围为[0,cueLength-1]
        throw outOfRange(); // 当待删结点不存在时，抛出异常
    pre = getPosition(i-1);
    p = pre->next; // p是真正的待删结点
    if (p == tail){ // 待删结点为尾节点，则修改尾指针
        tail = pre;
        pre->next = nullptr;
        delete p;
    }else{ // 修改指针并删除结点p
        pre->next = p->next;
        delete p;
    }
    curLength--;
}

template <class elemType>
void linkList<elemType>::headCreate(){
    Node *p;
    elemType value, flag;
    cout << "input elements, ended with:";
    cin >> flag; // 输入结束标志
    while (cin >> value, value != flag){
        p = new Node(value, head->next);
        head->next = p; // 结点p插在头结点的后面
        if (head == tail) tail = p; // 原链表为空，新结点p成为尾结点
        curLength++;
    }
}

template <class elemType>
void linkList<elemType>::tailCreate(){
    Node *p;
    elemType value, flag;
    cout << "input elements, ended with:";
    cin >> flag; // 输入结束标志
    while (cin >> value, value != flag){
        p = new Node(value, nullptr);
        tail->next = p; // 结点p插在尾结点的后面
        tail = p; // 结点p成为新的尾结点
        curLength++;
    }
}

template <class elemType>
void linkList<elemType>::inverse(){
    Node *p, *tmp;
    p = head->next; // p为工作指针指向首元结点
    head->next = nullptr; // 头结点的指针域置空，构成空链表
    if(p) tail = p; // 逆置后，原首元结点将变为尾节点
    while(p){
        tmp = p->next; // 暂存p的后继
        p->next = head->next;
        head->next = p; // 结点p插在头结点的后面
        p = tmp; // 继续处理下一个结点
    }
}

template <class elemType>
linkList<elemType>* linkList<elemType>::Union(linkList<elemType> *lb){
    Node *pa, *pb, *pc;
    linkList<elemType>* lc = this; // lc利用la的空间
    pa = head->next; head->next = nullptr; // la头结点的指针域置为nullptr，构成空链表
    pb = (lb->head)->next; (lb->head)->next = nullptr; // lb头结点的指针域置为nullptr，构成空链表
    pc = lc->head; // lc直接利用la的头结点
    while (pa && pb){ // 如果la和lb都非空
        if (pa->data <= pb->data){ // pa所指结点用尾插法插入lc中
            pc->next = pa;
            pc = pa;
            pa = pa->next;
        }else{ // pb所指结点用尾插法插入lc中
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
    }
    if(pa){ // 若pa未到表尾，则将pc指向pa
        pc->next = pa;
        lc->tail = tail;
    }else{// 若pb未到表尾，则将pc指向pb
        pc->next = pb;
        lc->tail = lb->tail;
    }
    lc->curLength = curLength + lb->curLength;
    delete lb;
    return lc;
}