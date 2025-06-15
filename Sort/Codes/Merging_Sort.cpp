#include <iostream>
using namespace std;

template <class Type>
void merge(Type R[], Type temp[], int low, int mid, int high){
    int i = low, j = mid, k = 0;
    while(i < mid && j <= high){ // R中记录由小到大复制到temp中
        if(R[i] < R[j]) temp[k++] = R[i++];
        else temp[k++] = R[j++]; // 将较小的元素放入临时数组
    }
    while(i < mid) temp[k++] = R[i++]; // 将左半部分剩余元素复制到temp
    while(j <= high) temp[k++] = R[j++]; // 将右半部分剩余元素复制到temp
    for(i = 0, k = low; k <= high)
        R[k++] = temp[i++]; // 将临时数组中的元素复制回原数组
}

template <class Type>
void mergeSort(Type R[], Type temp[], int low, int high){
    if (low == high) return; // 如果区间无效，直接返回
    int mid = (low + high) / 2; // 计算中间位置
    mergeSort(R, temp, low, mid); // 对左半部分递归排序
    mergeSort(R, temp, mid + 1, high); // 对右半部分递归排序
    merge(R, temp, low, mid + 1, high); // 合并两个已排序的子数组
}

template <class Type>
void mergeSort(Type R[], int size){
    Type* temp = new Type[size]; // 创建临时数组
    mergeSort(R, temp, 0, size - 1); // 调用归并排序
    delete[] temp; // 释放临时数组
}