#include <iostream>
#include "List.h"
using namespace std;


template <class elemType> // elemType为顺序表存储的元素类型
class seqList : public seqList<elemType>{
private:
    elemType *data; // 动态数组
    int curLength; // 当前顺序表中存储的元素个数
    int maxSize; // 顺序表的最大长度
    void resize(); // 表满时扩大表空间
public:
    seqList(int initSize = 10); // 构造函数
    seqList(seqList &sl); // 复制构造函数
    ~seqList(){delete [] data;} // 析构函数
    void clear(){curLength = 0;} // 清空表，只需要设置curLength = 0即可
    bool empty()const{return curLength == 0;} // 判空
    int size()const{return curLength;} // 返回顺序表中当前存储元素的个数
    void traverse()const; // 遍历顺序表
    void inverse(); // 逆置顺序表
    void insert(int i, const elemType &value); // 在位序i处插入值为value的元素，表长+1
    void remove(int i); // 删除位序i处的元素，表长-1
    int search(const elemType &value)const; // 查找值为value的元素第一次出现的位序
    bool Union(seqList<elemType> &B); // 合并顺序表A和B，结果存储在A中
    elemType visit(int i)const; // 访问位序为i的元素的值，“位序0”表示第一个元素
};

template <class elemType>
seqList<elemType>::seqList(int initSize = 100){
    if (initSize <= 0) throw badSize();
    maxSize = initSize;
    data = new elemType[maxSize];
    curLength = 0;
}

template <class elemType>
seqList<elemType>::seqList(seqList& sl){
    maxSize = sl.maxSize;
    curLength = sl.curLength;
    data = new elemType[maxSize];
    for (int i = 0; i < curLength; ++i){
        data[i] = sl.data[i];
    }
}

template <class elemType>
void seqList<elemType>::traverse()const{
    if(empty()){cout<<"is empty"<<endl;} // 空表没有元素
    else{
        cout<<"output element:"<<endl;
        for (int i = 0; i < curLength; i++){ // 依次访问顺序表中的所有元素
            cout<<data[i]<<" ";
        }
        cout<<endl;
    }
}

template <class elemType>
int seqList<elemType>::search(const elemType& value)const{
    for (int i = 0; i < curLength; i++){
        if (value == data[i]){return i;}
    }
    return -1;
}

template <class elemType>
void seqList<elemType>::insert(int i, const elemType& value){
    if (i < 0 || i > curLength) throw outOfRange(); // 合法的插入范围为[0,curLength]
    if (curLength == maxSize) resize();
    for (int j = curLength; j > i; j--){
        data[j] = data[j-1]; // 下标在[curLength-1,i]范围内的元素往后移动一步
    }
    data[i] = value; // 将值为value的元素放入位序为i的位置
    ++curlength; // 表的实际长度+1
}

template <class elemType>
void seqList<elemType>::remove(int i){
    if ( i < 0 || i > curLength - 1) throw outOfRange(); // 合法的删除范围为[0,curLength-1]
    for (int j = i; j < curLength - 1; j++){
        data[j] = data[j+1]; // 下标在[i+1,curLength-1]范围内的元素往前移动一步
    }
    --curLength; // 表的实际长度-1
}

template <class elemType>
void seqList<elemType>::inverse(){
    elemType tmp;
    for (int i = 0; i < curLength/2; i++){
        tmp = data[i];
        data[i] = data[curLength - 1];
        data[curLength - 1] = tmp;
    }
}

template <class elemType>
void seqList<elemType>::resize(){
    elemType* p = data; // p指向原顺序表空间
    maxSize *= 2; // 表空间扩大2倍
    data = new elemType[maxSize]; // data指向新的表空间
    for (int i = 0; i < curLength; i++){
        data[i] = p[i]; // 复制元素
    }
    delete [] p;
}

template <class elemType>
bool seqList<elemType>::Union(seqList<elemType> &B){
    int m, n, k, i, just;
    m = this->curLength; // 当前对象为线性表A
    n = B.curLength; // m, n分别为线性表A和B的长度
    k = m + n - 1; // k为结果线性表的工作指针(下标)
    i = m-1, j = n-1; // i, j分别为线性表A和B的工作指针(下标)
    if (m + n > this->maxSize){resize();} // 判断表A的空间是否足够大，如果不够则需要扩大表空间
    while (i >= 0 && j >= 0){ // 合并顺序表，直到一个表为空
        if (data[i] >= B.data[j]){
            data[k--] = data[i--]; // 默认当前对象,this指针可省略
        }else{data[k--] = B.data[j--];}
    }
    while (j >= 0){ // 将表B中剩余元素复制到表A中
        data[k--] = B.data[j--]
    }
    curLength = m+n; // 修改表A长度
    return true;
}