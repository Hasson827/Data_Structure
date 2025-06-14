#include <iostream>
using namespace std;

template <class Type>
void straightInsertSort(Type R[], int size){
    int pos, j; // pos为待插入位置记录
    Type temp;
    for(pos = 1, pos < size; pos++){
        temp = R[pos]; // 记录待插入的元素
        for(j = pos - 1; j >= 0 && R[j] > temp; j--) // 从后往前查找插入位置
            R[j + 1] = R[j]; // 将R[j]后移
        R[j + 1] = temp; // 插入R[pos]到正确位置
    }
}

template <class Type>
void binaryInsertSort(Type R[], int size){
    int pos, j, low, high, mid;
    Type temp;
    for(pos = 1; pos < size; pos++){
        temp = R[pos];
        low = 0; high = pos - 1;
        while(low <= high){
            mid = (low + high) / 2; // 查找中间位置
            if(R[mid] > temp) high = mid - 1; // 在左半边继续查找
            else low = mid + 1; // 在右半边继续查找
        }
        for(j = pos - 1; j >= low; j--)
            R[j + 1] = R[j]; // 将R[j]后移
        R[low] = temp; // 插入R[pos]到正确位置
    }
}