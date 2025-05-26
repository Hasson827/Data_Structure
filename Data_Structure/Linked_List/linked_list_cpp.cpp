#include <iostream>
using namespace std;

class ListNode {
public:
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class LinkedList {
public:
    ListNode* head;
    LinkedList() : head(nullptr) {} // 构造函数

    ~LinkedList(){
        ListNode* curr = head;
        while (curr){
            ListNode* next_node = curr->next; // 保存下一个节点
            delete curr; // 删除当前节点
            curr = next_node; // 移动到下一个节点
        }
        head = nullptr; // 确保头指针为nullptr
    }

    void addAtTail(int val) {
        if (!head) {
            head = new ListNode(val);
            return;
        }
        ListNode* curr = head;
        while (curr->next) curr = curr->next;
        curr->next = new ListNode(val);
    }

    void addAtHead(int val){
        ListNode* new_node = new ListNode(val);
        new_node->next = head;
        head = new_node;
    }

    void deleteNode(int val){
        ListNode* curr = head;
        ListNode* prev = nullptr;
        while (curr && curr->val != val){
            prev = curr;
            curr = curr->next;
        }
        if (!curr){return;} // Value not found
        if (prev){
            prev->next = curr->next; // 连接前一个节点和后一个节点
        } else {
            head = curr->next; // 如果是删除头节点
        }
        delete curr; // 释放当前节点的内存
    }

    void updateNode(int old_val, int new_val){
        ListNode* curr = head;
        while (curr){
            if (curr->val == old_val){
                curr->val = new_val;
                return;
            }
            curr = curr->next;
        }
    }

    void reverseList(){
        ListNode* prev = nullptr;
        ListNode* curr = head;
        ListNode* next = nullptr;

        while(curr){
            next = curr->next; // 保存下一个节点
            curr->next = prev; // 反转当前节点的指针
            prev = curr; // 移动prev到当前节点
            curr = next; // 移动到下一个节点
        }
        head = prev; // 更新头指针为新的头节点
    }

    void print() {
        ListNode* curr = head;
        while (curr) {
            cout << curr->val << " -> ";
            curr = curr->next;
        }
        cout << "NULL" << endl;
    }
};

int main() {
    LinkedList list;
    list.addAtTail(0);
    list.addAtTail(1);
    list.addAtTail(2);
    list.print();
    list.addAtHead(-1);
    list.addAtHead(-2);
    list.print();
    list.deleteNode(1);
    list.print();
    list.updateNode(-2, 10);
    list.print();
    list.reverseList();
    list.print();
    // Destructor will automatically clean up the list
    return 0;
}