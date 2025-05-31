#include <iostream>
#include <exception>
using namespace std;

// 异常类定义
class outOfRange : public exception {
public:
    const char* what() const throw() { return "ERROR! OUT OF RANGE.\n"; }
};

class badSize : public exception {
public:
    const char* what() const throw() { return "ERROR! BAD SIZE.\n"; }
};

// 抽象基类
template<class T>
class List {
public:
    virtual void clear() = 0;
    virtual bool empty() const = 0;
    virtual int size() const = 0;
    virtual void insert(int i, const T &value) = 0;
    virtual void remove(int i) = 0;
    virtual int search(const T &value) const = 0;
    virtual T visit(int i) const = 0;
    virtual void traverse() const = 0;
    virtual void inverse() = 0;
    virtual ~List() {}
};

// 顺序表实现
template <class elemType>
class seqList : public List<elemType> {
private:
    elemType *data;
    int curLength;
    int maxSize;
    void resize() {
        elemType* p = data;
        maxSize *= 2;
        data = new elemType[maxSize];
        for (int i = 0; i < curLength; i++)
            data[i] = p[i];
        delete[] p;
    }
public:
    seqList(int initSize = 100) {
        if (initSize <= 0) throw badSize();
        maxSize = initSize;
        data = new elemType[maxSize];
        curLength = 0;
    }

    seqList(const seqList& sl) {
        maxSize = sl.maxSize;
        curLength = sl.curLength;
        data = new elemType[maxSize];
        for (int i = 0; i < curLength; ++i)
            data[i] = sl.data[i];
    }

    ~seqList() { delete[] data; }

    void clear() { curLength = 0; }
    bool empty() const { return curLength == 0; }
    int size() const { return curLength; }

    void traverse() const {
        if (empty()) cout << "is empty" << endl;
        else {
            cout << "output element: " << endl;
            for (int i = 0; i < curLength; i++)
                cout << data[i] << " ";
            cout << endl;
        }
    }

    int search(const elemType& value) const {
        for (int i = 0; i < curLength; i++)
            if (value == data[i]) return i;
        return -1;
    }

    void insert(int i, const elemType& value) {
        if (i < 0 || i > curLength) throw outOfRange();
        if (curLength == maxSize) resize();
        for (int j = curLength; j > i; j--)
            data[j] = data[j-1];
        data[i] = value;
        ++curLength;
    }

    void remove(int i) {
        if (i < 0 || i > curLength - 1) throw outOfRange();
        for (int j = i; j < curLength - 1; j++)
            data[j] = data[j+1];
        --curLength;
    }

    elemType visit(int i) const {
        if (i < 0 || i >= curLength) throw outOfRange();
        return data[i];
    }

    void inverse() {
        elemType tmp;
        for (int i = 0; i < curLength/2; i++) {
            tmp = data[i];
            data[i] = data[curLength-1-i];
            data[curLength-1-i] = tmp;
        }
    }
};

// 单链表节点结构
template <class elemType>
struct Node {
    elemType data;
    Node* next;
    Node(const elemType value, Node* p = nullptr) : data(value), next(p) {}
    Node(Node* p = nullptr) : next(p) {}
};

// 单链表实现
template <class elemType>
class linkList : public List<elemType> {
private:
    struct Node {
        elemType data;
        Node* next;
        Node(const elemType value, Node* p = nullptr) : data(value), next(p) {}
        Node(Node* p = nullptr) : next(p) {}
    };
    Node* head;
    Node* tail;
    int curLength;

    Node* getPosition(int i) const {
        if (i < -1 || i > curLength-1) return nullptr;
        Node* p = head;
        int count = 0;
        while (count <= i) {
            p = p->next;
            count++;
        }
        return p;
    }

public:
    linkList() {
        head = tail = new Node();
        curLength = 0;
    }

    ~linkList() { clear(); delete head; }

    void clear() {
        Node *p = head->next, *tmp;
        while (p) {
            tmp = p;
            p = p->next;
            delete tmp;
        }
        head->next = nullptr;
        tail = head;
        curLength = 0;
    }

    bool empty() const { return head->next == nullptr; }
    int size() const { return curLength; }

    void traverse() const {
        Node* p = head->next;
        while (p) {
            cout << p->data << " ";
            p = p->next;
        }
        cout << endl;
    }

    int search(const elemType& value) const {
        Node* p = head->next;
        int count = 0;
        while (p && p->data != value) {
            p = p->next;
            count++;
        }
        return p ? count : -1;
    }

    void insert(int i, const elemType& value) {
        if (i < 0 || i > curLength) throw outOfRange();
        Node* p = getPosition(i-1);
        Node* q = new Node(value, p->next);
        p->next = q;
        if (p == tail) tail = q;
        curLength++;
    }

    void remove(int i) {
        if (i < 0 || i > curLength-1) throw outOfRange();
        Node* pre = getPosition(i-1);
        Node* p = pre->next;
        if (p == tail) {
            tail = pre;
            pre->next = nullptr;
        } else {
            pre->next = p->next;
        }
        delete p;
        curLength--;
    }

    elemType visit(int i) const {
        if (i < 0 || i >= curLength) throw outOfRange();
        Node* p = head->next;
        for (int count = 0; count < i; count++)
            p = p->next;
        return p->data;
    }

    void inverse() {
        Node* p = head->next;
        head->next = nullptr;
        if (p) tail = p;
        while (p) {
            Node* tmp = p->next;
            p->next = head->next;
            head->next = p;
            p = tmp;
        }
    }
};

// 双链表节点结构
template <class elemType>
struct DoubleNode {
    elemType data;
    DoubleNode* prior;
    DoubleNode* next;
    DoubleNode(const elemType value, DoubleNode* p = nullptr, DoubleNode* n = nullptr)
        : data(value), prior(p), next(n) {}
    DoubleNode() : next(nullptr), prior(nullptr) {}
};

// 双链表实现
template <class elemType>
class doubleLinkList : public List<elemType> {
private:
    struct DoubleNode {
        elemType data;
        DoubleNode* prior;
        DoubleNode* next;
        DoubleNode(const elemType value, DoubleNode* p = nullptr, DoubleNode* n = nullptr)
            : data(value), prior(p), next(n) {}
        DoubleNode() : next(nullptr), prior(nullptr) {}
    };
    DoubleNode* head;
    DoubleNode* tail;
    int curLength;

    DoubleNode* getPosition(int i) const {
        DoubleNode* p = head;
        int count = 0;
        if (i < -1 || i > curLength) return nullptr;
        while (count <= i) {
            p = p->next;
            count++;
        }
        return p;
    }

public:
    doubleLinkList() {
        head = new DoubleNode;
        tail = new DoubleNode;
        head->next = tail;
        tail->prior = head;
        curLength = 0;
    }

    ~doubleLinkList() {
        clear();
        delete head;
        delete tail;
    }

    void clear() {
        DoubleNode* p = head->next;
        DoubleNode* tmp;
        head->next = tail;
        tail->prior = head;
        while (p != tail) {
            tmp = p->next;
            delete p;
            p = tmp;
        }
        curLength = 0;
    }

    bool empty() const { return head->next == tail; }
    int size() const { return curLength; }

    void traverse() const {
        DoubleNode* p = head->next;
        while (p != tail) {
            cout << p->data << " ";
            p = p->next;
        }
        cout << endl;
    }

    int search(const elemType& value) const {
        DoubleNode* p = head->next;
        int count = 0;
        while (p != tail && p->data != value) {
            p = p->next;
            count++;
        }
        return (p != tail) ? count : -1;
    }

    void insert(int i, const elemType& value) {
        if (i < 0 || i > curLength) throw outOfRange();
        DoubleNode* p = getPosition(i);
        DoubleNode* tmp = new DoubleNode(value, p->prior, p);
        p->prior->next = tmp;
        p->prior = tmp;
        ++curLength;
    }

    void remove(int i) {
        if (i < 0 || i > curLength-1) throw outOfRange();
        DoubleNode* p = getPosition(i);
        p->prior->next = p->next;
        p->next->prior = p->prior;
        delete p;
        --curLength;
    }

    elemType visit(int i) const {
        if (i < 0 || i >= curLength) throw outOfRange();
        DoubleNode* p = head->next;
        for (int count = 0; count < i; count++)
            p = p->next;
        return p->data;
    }

    void inverse() {
        DoubleNode* tmp, *p = head->next;
        head->next = tail;
        tail->prior = head;
        while (p != tail) {
            tmp = p->next;
            p->next = head->next;
            p->prior = head;
            head->next->prior = p;
            head->next = p;
            p = tmp;
        }
    }
};

// 测试示例
int main() {
    // 顺序表测试
    seqList<int> sl(5);
    for(int i=0; i<5; i++) sl.insert(i, i*2);
    cout << "顺序表初始: "; sl.traverse();
    sl.inverse();
    cout << "顺序表逆置: "; sl.traverse();

    // 单链表测试
    linkList<int> ll;
    for(int i=0; i<5; i++) ll.insert(i, i+1);
    cout << "单链表初始: "; ll.traverse();
    ll.remove(2);
    cout << "删除第三个元素: "; ll.traverse();

    // 双链表测试
    doubleLinkList<int> dl;
    for(int i=0; i<3; i++) dl.insert(i, i*3);
    cout << "双链表初始: "; dl.traverse();
    dl.inverse();
    cout << "双链表逆置: "; dl.traverse();

    return 0;
}