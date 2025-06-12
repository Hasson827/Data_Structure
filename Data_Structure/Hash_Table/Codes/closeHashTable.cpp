#include <iostream>
#include "HashTable.h"
using namespace std;

template <class KeyType>
class closeHashTable : public hashTable<KeyType> {
    enum NodeState{EMPTY, ACTIVE, DELETED}; // 节点状态
    struct Node{
        KeyType key; // 键
        NodeState state; // 节点状态
        Node() : state(EMPTY) {} // 默认构造函数，状态为EMPTY
    };
    Node* data; // 散列表
    int maxSize; // 散列表的容量
    int curLength; // 散列表中元素的个数
    void resize(); // 扩展散列表
public:
    closeHashTable(int len = 11, int (*h)(const KeyType& k, int maxSize) = defaultHash);
    ~closeHashTable(){delete [] data;}
    int size() {return curLength;} // 返回散列表中元素的个数
    int capacity() {return maxSize;} // 返回散列表的容量
    bool search(const KeyType& k) const; // 查找元素
    int getPos(const KeyType& k) const; // 获取元素k的散列位置
    bool insert(const KeyType& k); // 插入元素
    bool remove(const KeyType& k); // 删除元素
    void print(); // 输出散列表
};

template <class KeyType>
closeHashTable<KeyType>::closeHashTable(int len, int (*h)(const KeyType& k, int maxSize)){
    maxSize = nextPrime(len); // 确保容量为质数
    data  = new Node[maxSize]; // 分配内存
    curLength = 0; // 初始化元素个数为0
    hash = h; // 设置散列函数
}

template <class KeyType>
bool closeHashTable<KeyType>::search(const KeyType& k) const {
    int offset = 1;
    int pos = hash(k, maxSize); // 关键字为k的元素的基地址
    while(data[pos].state == ACTIVE){ // 该位置状态为ACTIVE
        if(data[pos.key] != k){ // 关键字不匹配
            pos = (pos + offset) % maxSize; // 线性探测法，计算下一个位置
        }else{
            return true; // 找到元素，返回位置
        }
        return false; // 没有找到元素
    }
}

template <class KeyType>
int closeHashTable<KeyType>::getPos(const KeyType& k) const {
    int offset = 1;
    int pos = hash(k, maxSize); // 关键字为k的元素的基地址
    while(data[pos].state == ACTIVE){ // 该位置状态为ACTIVE
        if(data[pos].key != k){ // 关键字不匹配
            pos = (pos + offset) % maxSize; // 线性探测法，计算下一个位置
        }else{
            return pos; // 找到元素，返回位置
        }
    }
    return -1; // 没有找到元素
}

template <class KeyType>
bool closeHashTable<KeyType>::insert(const KeyType& k) {
    int offset = 1, pos;
    if(curLength >= maxSize / 2) resize(); // 如果装填因子过半，扩展散列表
    pos = hash(k, maxSize); // 关键字为k的元素的基地址
    while(data[pos].state == ACTIVE){ // 该位置状态为ACTIVE
        if(data[pos].key == k) return false; // 关键字已存在，插入失败
        else pos = (pos + offset) % maxSize; // 线性探测法，计算下一个位置
    }
    data[pos].key = k; // 插入元素
    data[pos].state = ACTIVE; // 设置状态为ACTIVE
    curLength++; // 元素个数加1
    return true; // 插入成功
}

template <class KeyType>
bool closeHashTable<KeyType>::remove(const KeyType& k) {
    int pos = getPos(k); // 获取元素k的散列位置
    if(pos == -1) return false; // 没有找到元素，删除失败
    data[pos].state = DELETED; // 设置状态为DELETED
    curLength--; // 元素个数减1
    return true; // 删除成功
}

template <class KeyType>
void closeHashTable<KeyType>::resize(){
    Node* tmp = data;
    int oldSize = maxSize; // 记录旧的散列表容量
    maxSize = nextPrime(2 * maxSize); // 扩展容量为原来的两倍
    data = new Node[maxSize]; // 分配新的内存
    curLength = 0; // 重置元素个数为0
    for(int i = 0; i < oldSize; i++){
        if(tmp[i].state == ACTIVE){
            insert(tmp[i].key);
            curLength++; // 插入旧散列表中的元素
        }
    }
    delete [] tmp; // 释放旧散列表的内存
}

template <class KeyType>
void closeHashTable<KeyType>::print(){
    int pos;
    cout << "输出闭散链表中的内容:" << endl;
    for(pos = 0; pos < maxSize; pos++){
        if(data[pos].state == ACTIVE){
            cout << pos << ": " << data[pos].key << "\t\t";
        }
    }
    cout << endl;
}