#include <iostream>
using namespace std;

template <class T>
class Queue{
public:
    virtual bool empty() const = 0; // 判空
    virtual void size() const = 0; // 队列大小
    virtual void enqueue(const T& x) = 0; // 入队
    virtual T dequeue() = 0; // 出队
    virtual T getHead() const = 0; // 获取队头元素
    virtual ~Queue() {} // 虚析构函数
};

class outOfRange : public exception{
public:
    const char* what() const throw(){
        return "Error! Out of range!";
    }
};
class badSize : public exception{
public:
    const char* what() const throw(){
        return "Error! Bad size!";
    }
};

template <class elemType>
class priorityQueue : public Queue<elemType>{
    int curLength; // 当前队列长度
    elemType* data; // 指向存放元素的数组
    int maxSize; // 队列的大小
    void resize(); // 扩大队列空间
    void siftDown(int parent); // 从parent位置向下调整优先级队列
    void siftUp(int postion); // 从position位置向上调整优先级队列
public:
    priorityQueue(int initSize = 100);
    priorityQueue(const elemType data[], int size);
    ~priorityQueue() { delete[] data; }
    bool empty() const {return curLength == 0;} // 判空
    int size() const {return curLength;} // 队列大小
    void buildHeap(); // 建堆
    void enQueue(const elemType& x); // 入队
    elemType deQueue(); // 出队
    elemType getHead() const{
        if(empty()) throw outOfRange();
        return data[1]; // 返回队头元素
    }
};

template <class elemType>
void priorityQueue<elemType>::enQueue(const elemType& x){
    if(curLength == maxSize - 1) resize(); // 队列已满，扩大空间
    data[++curLength] = x; // 将新元素放在队尾
    siftUp(curLength); // 调整堆
}

template <class elemType>
void priorityQueue<elemType>::siftUp(int position){
    elemType temp = data[position]; // 保存当前元素
    for(;position > 1 && temp < data[position / 2]; position /= 2)
        data[position] = data[position / 2]; // position位置元素比双亲小，双亲下移
    data[position] = temp; // 将当前元素放到合适的位置
}

template <class elemType>
elemType priorityQueue<elemType>::deQueue(){
    if(empty()) throw outOfRange();
    elemType min;
    min = data[1];
    data[1] = data[curLength--];
    siftDown(1);
    return min;
}

template <class elemType>
void priorityQueue<elemType>::siftDown(int parent){
    int child;
    elemType tmp = data[parent]; // 保存parent处结点
    for(; parent * 2 <= curLength; parent = child){
        child = parent * 2; // child用于记录较小的子结点
        if(child != curLength && data[child + 1] < data[child])
            child++; // 右孩子更小
        if(data[child] < tmp) data[parent] = data[child];
        else break;
    }
    data[parent] = tmp;
}

template <class elemType>
void priorityQueue<elemType>::buildHeap(){
    for(int i = curLength / 2; i > 0; i--){
        siftDown(i); // [curLength/2..1]从下标最大的分支结点开始调整
    }
}

template <class elemType>
priorityQueue<elemType>::priorityQueue(int initSize = 100){
    if(initSize <= 0) throw badSize();
    data = new elemType[initSize];
    maxSize = initSize;
    curLength = 0;
}

template <class elemType>
priorityQueue<elemType>::priorityQueue(const elemType *items, int size):
maxSize(size + 10), curLength(size){
    data = new elemType[maxSize];
    for(int i = 0; i < size; i++)
        data[i + 1] = items[i]; // 复制元素
}

template <class elemType>
void priorityQueue<elemType>::resize(){
    elemType* tmp = data; // tmp指向原堆空间
    maxSize *= 2; // 扩大堆空间
    data = new elemType[maxSize]; // 申请新的堆空间
    for (int i = 0; i < curLength; ++i)
        data[i] = tmp[i]; // 复制元素
    delete [] tmp;
}