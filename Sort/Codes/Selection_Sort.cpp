#include <iostream>
using namespace std;

template <class Type>
void swap(Type &a, Type &b){
    Type temp = a;
    a = b;
    b = temp;
}

template <class Type>
void straightSelectionSort(Type R[], int size){
    int pos, min, j; // min为一趟排序中最小记录的下标
    for(pos = 0; pos < size - 1; pos++){
        min = pos;
        for(j = pos + 1; j < size; j++)
            if(R[j] < R[min]) min = j; // 找到最小记录的下标
        if(pos != min) swap(R[pos], R[min]); // 如果最小记录不是当前记录，则交换
    }
}

template <class Type>
void siftDown(Type R[], int pos, int size){
    int child;
    Type temp = R[pos];
    for(; pos * 2 + 1 < size; pos = child){
        child = pos * 2 + 1; // 左孩子
        if(child + 1 < size && R[child] < R[child + 1]) // 如果右孩子存在且大于左孩子
            child++; // 则选择右孩子
        if(R[child] > temp) R[pos] = R[child]; // 如果孩子大于父节点，则将孩子上移
        else break; // 否则停止
    }
    R[pos] = temp; // 将父节点放到正确位置
}

template <class Type>
void heapSor(Type R[], int size){
    for(int i = size / 2 - 1; i >= 0; i--) // 建立初始堆
        siftDown(R, i, size);
    for(int i = size - 1; i > 0; i--){
        swap(R[0], R[i]); // 将堆顶元素与最后一个元素交换
        siftDown(R, 0, i); // 对剩余的堆进行调整
    }
}