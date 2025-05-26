#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node* next;
} ListNode;

ListNode* create_node(int val) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

void add_at_head(ListNode** head, int val){
    ListNode* new_node = create_node(val);
    new_node->next = *head;
    *head = new_node;
}

void add_at_tail(ListNode* head, int val) {
    while (head->next){head = head->next;}
    head->next = create_node(val);
}

void delete_node(ListNode** head, int val){
    ListNode* current = *head;
    ListNode* previous = NULL;

    while (current && current->val != val) {
        previous = current;
        current = current->next;
    }
    if (!current){return;} // Value not found
    // 如果此时还没有return，则current指向要删除的节点
    if (previous) { // 如果不是删除头节点
        previous->next = current->next; // 连接前一个节点和后一个节点
    } else { // 如果是删除头节点
        *head = current->next; // 将头节点指向下一个节点
    }
    free(current); // 释放当前节点的内存
}

void update_node(ListNode* head, int old_val, int new_val){
    while (head){
        if (head == NULL){return;}
        else if (head->val == old_val){
            head->val = new_val;
            return;
        }else{head = head->next;}
    }
}

void reverse_list(ListNode** head){
    ListNode* prev = NULL;
    ListNode* current = *head;
    ListNode* next = NULL;

    while (current) {
        next = current->next; // 保存下一个节点
        current->next = prev; // 反转当前节点的指针
        prev = current; // 移动prev到当前节点
        current = next; // 移动到下一个节点
    }
    *head = prev; // 更新头指针为新的头节点
}

void print_list(ListNode* head) {
    while (head) {
        printf("%d -> ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

void delete_list(ListNode** head){
    ListNode* current = *head;
    ListNode* next_node;

    while (current) {
        next_node = current->next; // 保存下一个节点
        free(current); // 释放当前节点
        current = next_node; // 移动到下一个节点
    }
    *head = NULL; // 将头指针设置为NULL
}

int main() {
    ListNode* head = create_node(0);
    add_at_tail(head, 1);
    add_at_tail(head, 2);
    print_list(head);
    add_at_head(&head, -1);
    add_at_head(&head, -2);
    print_list(head);
    delete_node(&head, 1);
    print_list(head);
    update_node(head, -2, 10);
    print_list(head);
    reverse_list(&head);
    print_list(head);
    delete_list(&head);
    return 0;
}