#include <iostream>
using namespace std;

template <class Type>
void swap(Type &a, Type &b){
    Type temp = a;
    a = b;
    b = temp;
}

template <class Type>
void bubbleSort(Type R[], int size){
    int i, j;
    bool flag = true; // 标志变量，表示是否发生交换
    for(i = 1; i < size && flag; i++){
        flag = false; // 每次开始时假设没有交换
        for(j = 0; j < size - i; j++){
            if(R[j+1] < R[j]){
                swap(R[j], R[j+1]);
                flag = true; // 如果发生交换，设置标志为true
            }
        }
    }
}

template <class Type>
int partition(Type S[], int low, int high){
    Type temp = S[low]; // 暂存枢轴
    while(low != high){
        while(low < high && S[high] >= temp) high--; // 从右向左找第一个小于temp的元素
        if(low < high){
            S[low] = S[high]; // 将该元素移到左边
            low++; // 移动low指针
        }
        while (low < high && S[low] <= temp) low++; // 从左向右找第一个大于temp的元素
        if(low < high){
            S[high] = S[low]; // 将该元素移到右边
            high--; // 移动high指针
        }
    }
    S[low] = temp; // 将temp放到正确的位置
    return low; // 返回分区点
}

template <class Type>
void quickSort(Type S[], int low, int high){
    int pivot;
    if(low >= high) return; // 如果区间无效，直接返回
    pivot = partition(S, low, high); // 获取分区点
    quickSort(S, low, pivot - 1); // 对左半部分递归排序
    quickSort(S, pivot + 1, high); // 对右半部分递归排序
}

template <class Type>
void quickSort(Type S[], int size){
    quickSort(S, 0, size - 1); // 调用快速排序
}