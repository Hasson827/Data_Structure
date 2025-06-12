#include <exception>
#include <iostream>
using namespace std;

template<class T>
class List{
public:
    virtual void clear() = 0; // 清空线性表
    virtual bool empty()const = 0; // 判空，表空返回true，非空返回false
    virtual int size()const = 0; // 求线性表的长度
    virtual void insert(int i, const T &value) = 0; // 在位序i处插入值为value的元素
    virtual void remove(int i) = 0; // 删除位序i处的元素
    virtual int search(const T &value)const = 0; // 查找值为value的元素第一次出现的位序
    virtual T visit(int i)const = 0; // 查找位序为i的元素并返回其值
    virtual void traverse()const = 0; // 遍历线性表
    virtual void inverse() = 0; // 逆置线性表
    virtual ~List(){};
};

class outOfRange : public exception{ // 用于检查范围的有效性
public:
    const char* what()const throw(){return "ERROR! OUT OF RANGE.\n";}
} ;
class badSize : public exception{ // 用于检查长度的有效性
public:
    const char* what()const throw(){return "ERROR! BAD SIZE.\n";}
};