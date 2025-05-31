#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 错误代码定义
#define OUT_OF_RANGE_ERROR -1
#define BAD_SIZE_ERROR -2
#define SUCCESS 0

// 前向声明
typedef struct List List;

// 抽象基类接口
typedef struct {
    void (*clear)(List* self);
    bool (*empty)(const List* self);
    int (*size)(const List* self);
    int (*insert)(List* self, int i, int value);
    int (*remove)(List* self, int i);
    int (*search)(const List* self, int value);
    int (*visit)(const List* self, int i, int* result);
    void (*traverse)(const List* self);
    void (*inverse)(List* self);
    void (*destroy)(List* self);
} ListVTable;

// 基类结构
struct List {
    const ListVTable* vtable;
};

// ========== 顺序表实现 ==========
typedef struct {
    List base;
    int* data;
    int curLength;
    int maxSize;
} SeqList;

// 顺序表方法声明
void seqlist_clear(List* self);
bool seqlist_empty(const List* self);
int seqlist_size(const List* self);
int seqlist_insert(List* self, int i, int value);
int seqlist_remove(List* self, int i);
int seqlist_search(const List* self, int value);
int seqlist_visit(const List* self, int i, int* result);
void seqlist_traverse(const List* self);
void seqlist_inverse(List* self);
void seqlist_destroy(List* self);

// 顺序表虚函数表
static const ListVTable seqlist_vtable = {
    seqlist_clear,
    seqlist_empty,
    seqlist_size,
    seqlist_insert,
    seqlist_remove,
    seqlist_search,
    seqlist_visit,
    seqlist_traverse,
    seqlist_inverse,
    seqlist_destroy
};

// 顺序表resize函数
static int seqlist_resize(SeqList* sl) {
    int* new_data = (int*)malloc(sl->maxSize * 2 * sizeof(int));
    if (!new_data) return BAD_SIZE_ERROR;
    
    for (int i = 0; i < sl->curLength; i++) {
        new_data[i] = sl->data[i];
    }
    
    free(sl->data);
    sl->data = new_data;
    sl->maxSize *= 2;
    return SUCCESS;
}

// 顺序表构造函数
SeqList* seqlist_create(int initSize) {
    if (initSize <= 0) return NULL;
    
    SeqList* sl = (SeqList*)malloc(sizeof(SeqList));
    if (!sl) return NULL;
    
    sl->base.vtable = &seqlist_vtable;
    sl->data = (int*)malloc(initSize * sizeof(int));
    if (!sl->data) {
        free(sl);
        return NULL;
    }
    
    sl->maxSize = initSize;
    sl->curLength = 0;
    return sl;
}

// 顺序表方法实现
void seqlist_clear(List* self) {
    SeqList* sl = (SeqList*)self;
    sl->curLength = 0;
}

bool seqlist_empty(const List* self) {
    const SeqList* sl = (const SeqList*)self;
    return sl->curLength == 0;
}

int seqlist_size(const List* self) {
    const SeqList* sl = (const SeqList*)self;
    return sl->curLength;
}

int seqlist_insert(List* self, int i, int value) {
    SeqList* sl = (SeqList*)self;
    if (i < 0 || i > sl->curLength) return OUT_OF_RANGE_ERROR;
    
    if (sl->curLength == sl->maxSize) {
        if (seqlist_resize(sl) != SUCCESS) return BAD_SIZE_ERROR;
    }
    
    for (int j = sl->curLength; j > i; j--) {
        sl->data[j] = sl->data[j-1];
    }
    sl->data[i] = value;
    sl->curLength++;
    return SUCCESS;
}

int seqlist_remove(List* self, int i) {
    SeqList* sl = (SeqList*)self;
    if (i < 0 || i >= sl->curLength) return OUT_OF_RANGE_ERROR;
    
    for (int j = i; j < sl->curLength - 1; j++) {
        sl->data[j] = sl->data[j+1];
    }
    sl->curLength--;
    return SUCCESS;
}

int seqlist_search(const List* self, int value) {
    const SeqList* sl = (const SeqList*)self;
    for (int i = 0; i < sl->curLength; i++) {
        if (sl->data[i] == value) return i;
    }
    return -1;
}

int seqlist_visit(const List* self, int i, int* result) {
    const SeqList* sl = (const SeqList*)self;
    if (i < 0 || i >= sl->curLength) return OUT_OF_RANGE_ERROR;
    *result = sl->data[i];
    return SUCCESS;
}

void seqlist_traverse(const List* self) {
    const SeqList* sl = (const SeqList*)self;
    if (sl->curLength == 0) {
        printf("is empty\n");
    } else {
        printf("output element: \n");
        for (int i = 0; i < sl->curLength; i++) {
            printf("%d ", sl->data[i]);
        }
        printf("\n");
    }
}

void seqlist_inverse(List* self) {
    SeqList* sl = (SeqList*)self;
    for (int i = 0; i < sl->curLength / 2; i++) {
        int tmp = sl->data[i];
        sl->data[i] = sl->data[sl->curLength - 1 - i];
        sl->data[sl->curLength - 1 - i] = tmp;
    }
}

void seqlist_destroy(List* self) {
    SeqList* sl = (SeqList*)self;
    free(sl->data);
    free(sl);
}

// ========== 单链表实现 ==========
typedef struct LinkNode {
    int data;
    struct LinkNode* next;
} LinkNode;

typedef struct {
    List base;
    LinkNode* head;
    LinkNode* tail;
    int curLength;
} LinkList;

// 单链表方法声明
void linklist_clear(List* self);
bool linklist_empty(const List* self);
int linklist_size(const List* self);
int linklist_insert(List* self, int i, int value);
int linklist_remove(List* self, int i);
int linklist_search(const List* self, int value);
int linklist_visit(const List* self, int i, int* result);
void linklist_traverse(const List* self);
void linklist_inverse(List* self);
void linklist_destroy(List* self);

// 单链表虚函数表
static const ListVTable linklist_vtable = {
    linklist_clear,
    linklist_empty,
    linklist_size,
    linklist_insert,
    linklist_remove,
    linklist_search,
    linklist_visit,
    linklist_traverse,
    linklist_inverse,
    linklist_destroy
};

// 获取位置节点
static LinkNode* linklist_get_position(const LinkList* ll, int i) {
    if (i < -1 || i >= ll->curLength) return NULL;
    LinkNode* p = ll->head;
    for (int count = 0; count <= i; count++) {
        p = p->next;
    }
    return p;
}

// 单链表构造函数
LinkList* linklist_create() {
    LinkList* ll = (LinkList*)malloc(sizeof(LinkList));
    if (!ll) return NULL;
    
    ll->base.vtable = &linklist_vtable;
    ll->head = (LinkNode*)malloc(sizeof(LinkNode));
    if (!ll->head) {
        free(ll);
        return NULL;
    }
    
    ll->head->next = NULL;
    ll->tail = ll->head;
    ll->curLength = 0;
    return ll;
}

// 单链表方法实现
void linklist_clear(List* self) {
    LinkList* ll = (LinkList*)self;
    LinkNode* p = ll->head->next;
    while (p) {
        LinkNode* tmp = p;
        p = p->next;
        free(tmp);
    }
    ll->head->next = NULL;
    ll->tail = ll->head;
    ll->curLength = 0;
}

bool linklist_empty(const List* self) {
    const LinkList* ll = (const LinkList*)self;
    return ll->head->next == NULL;
}

int linklist_size(const List* self) {
    const LinkList* ll = (const LinkList*)self;
    return ll->curLength;
}

int linklist_insert(List* self, int i, int value) {
    LinkList* ll = (LinkList*)self;
    if (i < 0 || i > ll->curLength) return OUT_OF_RANGE_ERROR;
    
    LinkNode* p = linklist_get_position(ll, i - 1);
    if (!p) return OUT_OF_RANGE_ERROR;
    
    LinkNode* new_node = (LinkNode*)malloc(sizeof(LinkNode));
    if (!new_node) return BAD_SIZE_ERROR;
    
    new_node->data = value;
    new_node->next = p->next;
    p->next = new_node;
    
    if (p == ll->tail) ll->tail = new_node;
    ll->curLength++;
    return SUCCESS;
}

int linklist_remove(List* self, int i) {
    LinkList* ll = (LinkList*)self;
    if (i < 0 || i >= ll->curLength) return OUT_OF_RANGE_ERROR;
    
    LinkNode* pre = linklist_get_position(ll, i - 1);
    if (!pre || !pre->next) return OUT_OF_RANGE_ERROR;
    
    LinkNode* p = pre->next;
    if (p == ll->tail) {
        ll->tail = pre;
        pre->next = NULL;
    } else {
        pre->next = p->next;
    }
    
    free(p);
    ll->curLength--;
    return SUCCESS;
}

int linklist_search(const List* self, int value) {
    const LinkList* ll = (const LinkList*)self;
    LinkNode* p = ll->head->next;
    int count = 0;
    while (p && p->data != value) {
        p = p->next;
        count++;
    }
    return p ? count : -1;
}

int linklist_visit(const List* self, int i, int* result) {
    const LinkList* ll = (const LinkList*)self;
    if (i < 0 || i >= ll->curLength) return OUT_OF_RANGE_ERROR;
    
    LinkNode* p = ll->head->next;
    for (int count = 0; count < i; count++) {
        p = p->next;
    }
    *result = p->data;
    return SUCCESS;
}

void linklist_traverse(const List* self) {
    const LinkList* ll = (const LinkList*)self;
    LinkNode* p = ll->head->next;
    while (p) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

void linklist_inverse(List* self) {
    LinkList* ll = (LinkList*)self;
    LinkNode* p = ll->head->next;
    ll->head->next = NULL;
    if (p) ll->tail = p;
    
    while (p) {
        LinkNode* tmp = p->next;
        p->next = ll->head->next;
        ll->head->next = p;
        p = tmp;
    }
}

void linklist_destroy(List* self) {
    linklist_clear(self);
    LinkList* ll = (LinkList*)self;
    free(ll->head);
    free(ll);
}

// ========== 双链表实现 ==========
typedef struct DoubleLinkNode {
    int data;
    struct DoubleLinkNode* prior;
    struct DoubleLinkNode* next;
} DoubleLinkNode;

typedef struct {
    List base;
    DoubleLinkNode* head;
    DoubleLinkNode* tail;
    int curLength;
} DoubleLinkList;

// 双链表方法声明
void doublelinklist_clear(List* self);
bool doublelinklist_empty(const List* self);
int doublelinklist_size(const List* self);
int doublelinklist_insert(List* self, int i, int value);
int doublelinklist_remove(List* self, int i);
int doublelinklist_search(const List* self, int value);
int doublelinklist_visit(const List* self, int i, int* result);
void doublelinklist_traverse(const List* self);
void doublelinklist_inverse(List* self);
void doublelinklist_destroy(List* self);

// 双链表虚函数表
static const ListVTable doublelinklist_vtable = {
    doublelinklist_clear,
    doublelinklist_empty,
    doublelinklist_size,
    doublelinklist_insert,
    doublelinklist_remove,
    doublelinklist_search,
    doublelinklist_visit,
    doublelinklist_traverse,
    doublelinklist_inverse,
    doublelinklist_destroy
};

// 获取位置节点
static DoubleLinkNode* doublelinklist_get_position(const DoubleLinkList* dll, int i) {
    if (i < -1 || i > dll->curLength) return NULL;
    DoubleLinkNode* p = dll->head;
    for (int count = 0; count <= i; count++) {
        p = p->next;
    }
    return p;
}

// 双链表构造函数
DoubleLinkList* doublelinklist_create() {
    DoubleLinkList* dll = (DoubleLinkList*)malloc(sizeof(DoubleLinkList));
    if (!dll) return NULL;
    
    dll->base.vtable = &doublelinklist_vtable;
    dll->head = (DoubleLinkNode*)malloc(sizeof(DoubleLinkNode));
    dll->tail = (DoubleLinkNode*)malloc(sizeof(DoubleLinkNode));
    
    if (!dll->head || !dll->tail) {
        free(dll->head);
        free(dll->tail);
        free(dll);
        return NULL;
    }
    
    dll->head->next = dll->tail;
    dll->head->prior = NULL;
    dll->tail->prior = dll->head;
    dll->tail->next = NULL;
    dll->curLength = 0;
    return dll;
}

// 双链表方法实现
void doublelinklist_clear(List* self) {
    DoubleLinkList* dll = (DoubleLinkList*)self;
    DoubleLinkNode* p = dll->head->next;
    dll->head->next = dll->tail;
    dll->tail->prior = dll->head;
    
    while (p != dll->tail) {
        DoubleLinkNode* tmp = p->next;
        free(p);
        p = tmp;
    }
    dll->curLength = 0;
}

bool doublelinklist_empty(const List* self) {
    const DoubleLinkList* dll = (const DoubleLinkList*)self;
    return dll->head->next == dll->tail;
}

int doublelinklist_size(const List* self) {
    const DoubleLinkList* dll = (const DoubleLinkList*)self;
    return dll->curLength;
}

int doublelinklist_insert(List* self, int i, int value) {
    DoubleLinkList* dll = (DoubleLinkList*)self;
    if (i < 0 || i > dll->curLength) return OUT_OF_RANGE_ERROR;
    
    DoubleLinkNode* p = doublelinklist_get_position(dll, i);
    if (!p) return OUT_OF_RANGE_ERROR;
    
    DoubleLinkNode* new_node = (DoubleLinkNode*)malloc(sizeof(DoubleLinkNode));
    if (!new_node) return BAD_SIZE_ERROR;
    
    new_node->data = value;
    new_node->prior = p->prior;
    new_node->next = p;
    p->prior->next = new_node;
    p->prior = new_node;
    
    dll->curLength++;
    return SUCCESS;
}

int doublelinklist_remove(List* self, int i) {
    DoubleLinkList* dll = (DoubleLinkList*)self;
    if (i < 0 || i >= dll->curLength) return OUT_OF_RANGE_ERROR;
    
    DoubleLinkNode* p = doublelinklist_get_position(dll, i);
    if (!p || p == dll->tail) return OUT_OF_RANGE_ERROR;
    
    p->prior->next = p->next;
    p->next->prior = p->prior;
    free(p);
    dll->curLength--;
    return SUCCESS;
}

int doublelinklist_search(const List* self, int value) {
    const DoubleLinkList* dll = (const DoubleLinkList*)self;
    DoubleLinkNode* p = dll->head->next;
    int count = 0;
    while (p != dll->tail && p->data != value) {
        p = p->next;
        count++;
    }
    return (p != dll->tail) ? count : -1;
}

int doublelinklist_visit(const List* self, int i, int* result) {
    const DoubleLinkList* dll = (const DoubleLinkList*)self;
    if (i < 0 || i >= dll->curLength) return OUT_OF_RANGE_ERROR;
    
    DoubleLinkNode* p = dll->head->next;
    for (int count = 0; count < i; count++) {
        p = p->next;
    }
    *result = p->data;
    return SUCCESS;
}

void doublelinklist_traverse(const List* self) {
    const DoubleLinkList* dll = (const DoubleLinkList*)self;
    DoubleLinkNode* p = dll->head->next;
    while (p != dll->tail) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

void doublelinklist_inverse(List* self) {
    DoubleLinkList* dll = (DoubleLinkList*)self;
    DoubleLinkNode* p = dll->head->next;
    dll->head->next = dll->tail;
    dll->tail->prior = dll->head;
    
    while (p != dll->tail) {
        DoubleLinkNode* tmp = p->next;
        p->next = dll->head->next;
        p->prior = dll->head;
        dll->head->next->prior = p;
        dll->head->next = p;
        p = tmp;
    }
}

void doublelinklist_destroy(List* self) {
    doublelinklist_clear(self);
    DoubleLinkList* dll = (DoubleLinkList*)self;
    free(dll->head);
    free(dll->tail);
    free(dll);
}

// ========== 测试程序 ==========
int main() {
    // 顺序表测试
    SeqList* sl = seqlist_create(5);
    if (sl) {
        for (int i = 0; i < 5; i++) {
            sl->base.vtable->insert((List*)sl, i, i * 2);
        }
        printf("顺序表初始: ");
        sl->base.vtable->traverse((List*)sl);
        sl->base.vtable->inverse((List*)sl);
        printf("顺序表逆置: ");
        sl->base.vtable->traverse((List*)sl);
        sl->base.vtable->destroy((List*)sl);
    }

    // 单链表测试
    LinkList* ll = linklist_create();
    if (ll) {
        for (int i = 0; i < 5; i++) {
            ll->base.vtable->insert((List*)ll, i, i + 1);
        }
        printf("单链表初始: ");
        ll->base.vtable->traverse((List*)ll);
        ll->base.vtable->remove((List*)ll, 2);
        printf("删除第三个元素: ");
        ll->base.vtable->traverse((List*)ll);
        ll->base.vtable->destroy((List*)ll);
    }

    // 双链表测试
    DoubleLinkList* dll = doublelinklist_create();
    if (dll) {
        for (int i = 0; i < 3; i++) {
            dll->base.vtable->insert((List*)dll, i, i * 3);
        }
        printf("双链表初始: ");
        dll->base.vtable->traverse((List*)dll);
        dll->base.vtable->inverse((List*)dll);
        printf("双链表逆置: ");
        dll->base.vtable->traverse((List*)dll);
        dll->base.vtable->destroy((List*)dll);
    }

    return 0;
}