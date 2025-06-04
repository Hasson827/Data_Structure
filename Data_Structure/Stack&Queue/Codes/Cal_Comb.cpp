/*
设整形数组A中有size个整数，输出从这size个数中取出k个数的全部组合
分析：从数组A中选出k个元素，为了避免重复和漏选，可分别求出包括A[0]和不包括A[0]的两种情况，
然后递归求解。包括A[0]的情况是将A[0]放入结果集，然后从A[1]到A[size-1]中选出k-1个元素；
不包括A[0]的情况是从A[1]到A[size-1]中选出k个元素。
*/
#include <iostream>
using namespace std;

class Comb{
    int* A; // 数组A的大小为size
    int* B; // 数组B用于存储从A中选取的k个元素
    int size; // 数组A的大小
    int k; // 需要选取的元素个数
public:
    Comb(int sizeValue, int kValue);
    ~Comb();
    void calculate(int i, int j, int kk);
};

Comb::Comb(int sizeValue, int kValue) {
    size = sizeValue;
    k = kValue;
    if (k > size || k <= 0){
        cout << "Invalid input: k must be between 0 and size." << endl;
        return;
    }
    A = new int[size];
    B = new int[size];
    for(int i = 0; i < size; i++){
        A[i] = i + 1; // 初始化数组A为1到size的整数
    }
}

Comb::~Comb() {
    delete[] A;
    delete[] B;
}

void Comb::calculate(int i, int j, int kk){
    if(kk == 0){ // 已经选取k个元素
        for(int i = 0; i < k; i++){
            cout << B[i] <<" ";
        }
        cout <<endl;
    }else if(i + kk - 1 < size){ // 还有元素可以选
        B[j++] = A[i]; // 选取当前元素A[i]
        calculate(i+1, j, kk-1); // 选取的元素中包含A[i]
        calculate(i+1, j-1, kk); // 选取的元素中不包含A[i]
    }
} // 调用形式：calculate(0,0,k);

int main(){
    int size, k;
    cout << "Enter the size of the array: ";
    cin >> size;
    cout << "Enter the number of elements to choose (k): ";
    cin >> k;

    Comb comb(size, k);
    comb.calculate(0, 0, k); // 开始计算组合

    return 0;
}