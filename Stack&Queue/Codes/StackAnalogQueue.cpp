/*
利用两个顺序栈s1,s2模拟一个队列，如何用栈的运算来实现队列的运算。
1. empty():判断队列是否为空
2. full():判断队列是否为满
3. enQueue():插入一个元素
4. deQueue():删除一个元素
*/
#include <iostream>
using namespace std;

//! 顺序栈的实现
template <class T>
class seqStack{
    T* data;
    int maxSize; // 栈的最大容量
    int top; // 栈顶指针
public:
    seqStack(int size); // 构造函数
    ~seqStack(){delete [] data;} // 析构函数
    bool push(T item); // 入栈
    bool pop(T* item); // 出栈
    bool empty(){return top == -1;} // 判断栈是否为空
    bool full(){return top == maxSize - 1;} // 判断栈是否为满
};
//! 构造函数
template <class T>
seqStack<T>::seqStack(int size){
    maxSize = size;
    top = -1;
    data = new T[maxSize];
}
//! 入栈
template <class T>
bool seqStack<T>::push(T item){
    if(full()){
        cout << "栈已满，无法入栈" << endl; 
        return false;
    }else{
        data[++top] = item;
        return true;
    }
}
//! 出栈
template <class T>
bool seqStack<T>::pop(T* item){
    if(empty()){
        cout << "栈为空，无法出栈" << endl;
        return false;
    }else{
        *item = data[top--];
        return true;
    }
}


//! 用顺序栈模拟实现队列
template <class T>
class stackAnalogQueue{
    int mSize; // 队列的大小
    seqStack<T> *s1, *s2; // 两个栈,s1为入栈，s2为出栈
public:
    stackAnalogQueue(int size){
        mSize = size / 2;
        s1 = new seqStack<T>(mSize);
        s2 = new seqStack<T>(mSize);
    }
    ~stackAnalogQueue(){
        delete s1;
        delete s2;
    }
    bool empty();
    bool full();
    bool enQueue(T item);
    bool deQueue(T* item);
};
//! 判空
template <class T>
bool stackAnalogQueue<T>::empty(){
    return s1->empty() && s2->empty();
}
//! 判满
template <class T>
bool stackAnalogQueue<T>::full(){
    return s1->full() && s2->full();
}
//! 入队
template <class T>
bool stackAnalogQueue<T>::enQueue(T item){
    T* temp = new T;
    if(full()){
        cout << "队列已满，无法入队" <<endl;
        delete temp;
        return false;
    }
    if(s1->full() && s2->empty()){ // s1满且s2空时，将s1中的元素全部转移到s2中
        while(!s1->empty()){
            s1->pop(temp);
            s2->push(*temp);
        }
    }
    s1->push(item);
    delete temp;
    return true;
}
//! 出队
template <class T>
bool stackAnalogQueue<T>::deQueue(T* item){
    T* temp = new T;
    if(empty()){
        cout << "队列为空，无法出队" << endl;
        delete temp;
        return false;
    }
    if(!s2->empty()){s2->pop(item);} // 如果s2不空，直接出栈
    else{ // s2为空
        while(!s1->empty()){ // s1非空时，将s1中的元素全部转移到s2中
            s1->pop(temp);
            s2->push(*temp);
        }
        s2->pop(item);
    }
    delete temp;
    return true;
}


int main(){ // 测试代码
    stackAnalogQueue<int> queue(10); // 创建一个大小为10的队列
    int item;
    
    queue.enQueue(1);
    queue.enQueue(2);
    queue.enQueue(3);
    queue.enQueue(4);
    
    while(!queue.empty()){
        queue.deQueue(&item);
        cout << "出队元素: " << item << endl;
    }
    
    return 0;
}