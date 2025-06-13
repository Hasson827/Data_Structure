#include <iostream>
#include <string>
using namespace std;

template <class T>
class huffmanTree{
    struct Node{
        T data; // 结点的数据域
        int weight; // 结点的权值
        int parent, left, right; // 双亲及左右孩子的下标
        Node(){
            weight = parent = left = right = 0;
        }
    };
    struct huffmanCode{
        T data;
        string code; // 保存data的哈夫曼编码
        huffmanCode(){
            code = "";
        }
    };
    Node* hfTree; // 顺序存储结构，保存哈夫曼树
    huffmanCode* hfCode; // 顺序存储结构，保存哈夫曼编码
    int size; // 叶结点个数
    void selectMin(int m, int& p); // 选出当前集合中的最小元素
public:
    huffmanTree(int initSize); // 构造函数
    ~huffmanTree(){delete [] hfTree; delete [] hfCode;}; // 析构函数
    void createHuffmanTree(const T* d, const double* w); // 创建哈夫曼树
    void huffmanEncoding(); // 获取哈夫曼编码
    void printHuffmanCode(); // 输出哈夫曼编码
};

template <class T>
huffmanTree<T>::huffmanTree(int initSize){
    size = initSize;
    hfTree = new Node[2 * size]; // 哈夫曼树的结点数为2*size-1
    hfCode = new huffmanCode[size]; // 哈夫曼编码的个数为size
}

template <class T>
void huffmanTree<T>::createHuffmanTree(const T* d, const double* w){
    int i, min1, min2; // 最小树、次最小树的下标
    for(i = size; i < 2 * size; ++i){ // 给size个叶结点赋值
        hfTree[i].data = d[i - size];
        hfTree[i].weight = w[i - size];
    }
    for (i = size - 1; i > 0; --i){ // 合并产生size-1个新结点
        // 选出parent的值为0且权值最小的两棵子树min1、min2作为结点i的左右孩子
        selectMin(i + 1, min1); hfTree[min1].parent = i;
        selectMin(i + 1, min2); hfTree[min2].parent = i;
        hfTree[i].weight = hfTree[min1].weight + hfTree[min2].weight;
        hfTree[i].left = min1;
        hfTree[i].right = min2;
    }
}

template <class T>
void huffmanTree<T>::selectMin(int m, int& p){
    int j = m;
    while(hfTree[j].parent != 0) j++; // 跳过已有双亲的结点
    for(p = j, j+=1; j < 2 * size; ++j){ // 向后扫描剩余元素
        if((hfTree[j].weight < hfTree[p].weight) && 0 == hfTree[j].parent)
            p = j; // 发现更小的记录，记录它的下标
    }
}

template <class T>
void huffmanTree<T>::huffmanEncoding(){
    int f, p; // p是当前正在处理的结点，f是p的双亲的下标
    for(int i = size; i < 2 * size; ++i){
        hfCode[i - size].data = hfTree[i].data;
        p = i;
        f = hfTree[p].parent;
        while (f){
            if(hfTree[f].left == p) // p是其双亲f的孩子，编码+'0'
                hfCode[i - size].code = '0' + hfCode[i - size].code;
            else // p是其双亲f的右孩子，编码+'1'
                hfCode[i - size].code = '1' + hfCode[i - size].code;
            p = f;
            f = hfTree[p].parent; // 继续向上追溯
        }
    }
}

template <class T>
void huffmanTree<T>::printHuffmanCode(){
    for(int i = 0; i < size; i++)
        cout << hfCode[i].data << ' ' << hfCode[i].code << endl;
}

int main(){
    char d[] = "GOLESD";
    double w[] = {4,6,1,2,1,1,2};
    huffmanTree<char> tree(7);
    tree.createHuffmanTree(d, w);
    tree.huffmanEncoding();
    tree.printHuffmanCode();
    return 0;
}