#include <iostream>
using namespace std;

template <class Type>
class hashTable{
public:
    virtual int size() = 0; // 散列表中元素的个数
    virtual int capacity() = 0; // 散列表的容量
    virtual bool search(const Type& k) const = 0; // 查找元素
    virtual bool insert(const Type& k) = 0; // 插入元素
    virtual bool remove(const Type& k) = 0; // 删除元素
    virtual void print() = 0; // 输出散列表
    virtual ~hashTable(){}
protected:
    int nextPrime(int n); // 求大于n的最小质数
    int (*hash)(const Type& k, int maxSize); // 散列函数
    static int defaultHash(const Type& k, int maxSize = capacity()); // 默认散列函数
};

template <class Type>
int hashTable<Type>::defaultHash(const Type& k, int maxSize) {
    int hashVal = k % maxSize; // 使用模运算计算散列值
    if(hashVal < 0) hashVal += maxSize; // 确保散列值为非负
    return hashVal;
}

template <class Type>
int hashTable<Type>::nextPrime(int n) {
    int i;
    if(n % 2 == 0) n++; // 确保n为奇数
    for( ; ; n += 2){
        for(i = 3; i * i <= n; i += 2){
            if(n % i == 0) break;
        }
        if(i * i > n) return n; // 找到大于n的最小质数
    }
}

