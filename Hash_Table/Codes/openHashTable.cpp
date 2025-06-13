#include <iostream>
#include "HashTable.h"
using namespace std;

template <class Type>
class openHashTable : public hashTable<Type> {
    struct Node {
        Type key; // 键
        Node* next; // 指向下一个节点的指针
        Node() : next(nullptr) {} // 默认构造函数
        Node(const Type& k) : key(k), next(nullptr) {} // 构造函数
    };
    Node** data; // 散列表
    int maxSize; // 散列表的容量
    int curLength; // 散列表中元素的个数
    void resize(); // 扩展散列表
public:
    openHashTable(int len = 11, int (*h)(const Type& k, int maxSize) = defaultHash);
    ~openHashTable();
    int size() { return curLength; } // 返回散列表中元素的个数
    int capacity() { return maxSize; } // 返回散列表的容量
    bool search(const Type& k) const; // 查找元素
    bool insert(const Type& k); // 插入元素
    bool remove(const Type& k); // 删除元素
    void print(); // 输出散列表
};

template <class Type>
openHashTable<Type>::openHashTable(int len, int (*h)(const Type& k, int maxSize)) {
    curLength = 0; // 初始化元素个数为0
    hash = h; // 设置散列函数
    maxSize = nextPrime(len); // 确保容量为质数
    data = new Node*[maxSize]; // 分配内存
    for (int i = 0; i < maxSize; ++i) 
        data[i] = new Node; // 为每个链表申请头结点   
}

template <class Type>
openHashTable<Type>::~openHashTable() {
    for (int i = 0; i < maxSize; ++i) {
        Node* current = data[i];
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete; // 释放每个节点的内存
        }
    }
    delete[] data; // 释放散列表数组的内存
}

template <class Type>
bool openHashTable<Type>::search(const Type& k) const {
    int pos = hash(k, maxSize); // 计算散列位置
    Node* current = data[pos]->next; // 跳过头结点
    while (current) {
        if (current->key == k) {
            return true; // 找到元素
        }
        current = current->next; // 移动到下一个节点
    }
    return false; // 没有找到元素
}

template <class Type>
bool openHashTable<Type>::insert(const Type& k){
    if(search(k)) return false; // 如果元素已存在，返回false
    if(curLength+1 >= maxSize) resize(); // 如果元素个数超过容量的一半，扩展散列表
    int pos = hash(k, maxSize); // 计算散列位置
    Node* newNode = new Node(k); // 创建新节点
    newNode->next = data[pos]->next; // 将新节点插入到链表头部
    data[pos]->next = newNode; // 更新头结点的指针
    curLength++; // 增加元素个数
    return true; // 插入成功
}

template <class Type>
bool openHashTable<Type>::remove(const Type& k) {
    int pos = hash(k, maxSize); // 计算散列位置
    Node *pre = data[pos], *p;
    while(pre->next != nullptr && pre->next->key != k)
        pre = pre->next; // 找到要删除的节点的前一个节点
    if(pre->next == nullptr) return false; // 如果没有找到，返回false
    else{
        p = pre->next; // 找到要删除的节点
        pre->next = p->next; // 更新前一个节点的指针
        delete p; // 释放要删除的节点内存
        curLength--; // 减少元素个数
        return true; // 删除成功
    }
}

template <class Type>
void openHashTable<Type>::resize(){
    Node **tmp = data, *p, *q;
    int i, pos, oldSize = maxSize;
    maxSize = nextPrime(2 * maxSize); // 扩展容量为原来的两倍
    data = new Node*[maxSize];
    for(i = 0; i < maxSize; i++)
        data[i] = new Node; // 为每个链表申请头结点
    for(i = 0; i < oldSize; i++){
        p = tmp[i]->next; // 获取旧链表的第一个节点
        while(p != nullptr) {
            pos = hash(p->key, maxSize); // 计算新位置
            q = p->next; // 保存下一个节点
            p->next = data[pos]->next; // 在新hash地址的表头插入p结点
            data[pos]->next = p; // 更新头结点的指针
            p = q; // 移动到下一个节点
        }
    }
    for(i = 0; i < oldSize; i++) delete tmp[i]; // 释放旧链表的内存
    delete[] tmp; // 释放旧散列表数组的内存
}

template <class Type>
void openHashTable<Type>::print() {
    Node* p;
    cout << "输出开散列表中的内容: " << endl;
    for(int i = 0; i < maxSize; i++){
        p = data[i]->next; // 跳过头结点
        cout << i << ":";
        while(p != nullptr) {
            cout << "-->" << p->key; // 输出当前节点的键
            p = p->next; // 移动到下一个节点
        }
    }
    cout << endl; // 换行
}