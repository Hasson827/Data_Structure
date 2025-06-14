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