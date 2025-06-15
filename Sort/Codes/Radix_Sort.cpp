#include <iostream>
using namespace std;

int getDigit(int key, int i){
    for(int j = 1; j < i; j++)
        key /= 10; // 将key右移i-1位
    return key % 10; // 返回第i位数字
}

const int radix = 10; // 基数为10
template <class Type>
void LSD(Type R[], int size, int i){
    Type* bucket = new Type[size]; // 创建桶数组
    int* position = new int[radix]; // 计数器
    int j, k;
    for(j = 0; j < radix; j++)
        position[j] = 0; // 初始化计数器
    for(j = 0; j < size; j++){
        k = getDigit(R[j], i); // 计算每个桶的容量
        position[k]++; // 计数器加1
    }
    for(j = 1; j < radix; j++) // 按照每个桶的容量，分配bucket数组的位置
        position[j] += position[j - 1] + position[j];
    for(j = size - 1; j >= 0; j--){
        k = getDigit(R[j], i); // 获取第i位数字
        bucket[--position[k]] = R[j]; // 将元素放入对应的桶中
    }
    for(j = 0; j < size; j++)
        R[j] = bucket[j]; // 将桶中的元素复制回原数组
    delete[] bucket; // 释放桶数组
    delete[] position; // 释放计数器数组
}

template <class Type>
void radixSort(Type R[], int size){
    int i, d = 1, max = R[0];
    for(i = 1; i < size; i++)
        if(R[i] > max) max = R[i]; // 找到最大值
    while(max = max / 10) d++; // 计算最大值的位数
    for(i = 1; i <= d; i++)
        LSD(R, size, i); // 对每一位进行LSD排序
}