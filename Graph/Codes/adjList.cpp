#include <iostream>
#include <queue>
#include "Graph.h"
using namespace std;

template <class VertexType, class EdgeType>
class adjList : public Graph<VertexType, EdgeType>{
    struct edgeNode{ // 边表结点类型
        int to; // 边的终点编号(在顶点表中的下标)
        EdgeType weight; // 边的权重
        edgeNode* next; // 指向下一条边表结点
        edgeNode(){}
        edgeNode(int t, EdgeType w, edgeNode* n = nullptr){
            to = t;
            weight = w;
            next = n;
        }
    };
    struct verNode{ // 顶点结点类型
        VertexType vertex; // 顶点信息
        edgeNode* firstEdge; // 指向第一个临界点的指针
        verNode(edgeNode* h = nullptr){
            firstEdge = h; // 初始化顶点结点
        }
    };
    verNode* verList; // 顶点表
    int* topOrder; // 保存拓扑排序，用于求关键路径
    void dfs(int start) const; // 从start号顶点出发深度优先遍历图
public:
    adjList(int size);
    ~adjList();
    void createGraph(const VertexType V[], const EdgeType E[]); // 创建图
    void printGraph() const; // 打印图
    bool searchEdge(int from, int to) const; // 查找边
    bool insertEdge(int from, int to, EdgeType w); // 插入边
    bool removeEdge(int from, int to); // 删除边
    void dfsTraverse() const; // 深度优先遍历
    void bfsTraverse() const; // 广度优先遍历
    void topSort() const; // 拓扑排序
    void prim(EdgeType noEdge); // Prim算法求最小生成树
    void kruskal() const; // Kruskal算法求最小生成树
    void printMst() const; // 打印最小生成树
    bool criticalPath() const; // 关键路径算法
    bool dijkastra(int start, EdgeType noEdge) const; // 求单源点最短路径
    bool printDijPath(int from, int to, int pre[]) const; // 输出源点到其他顶点的最短路径
};

template <class VertexType, class EdgeType>
adjList<VertexType, EdgeType>::adjList(int size){
    verNum = size;
    edgeNum = 0;
    verList = new verNode[verNum]; // 初始化顶点表
    topOrder = new int[verNum]; // 初始化拓扑排序数组
    visited = new bool[verNum]; // 访问标志数组
    TE = new mstEdge[verNum - 1]; // 最小生成树的边集
}

template <class VertexType, class EdgeType>
adjList<VertexType, EdgeType>::~adjList(){
    for(int i = 0; i < verNum; i++){
        edgeNode* p = verList[i].firstEdge;
        while(p != nullptr){
            edgeNode* temp = p;
            p = p->next;
            delete temp; // 删除边表结点
        }
    }
    delete [] verList; // 删除顶点表
    delete [] topOrder; // 删除拓扑排序数组
    delete [] visited; // 删除访问标志数组
    delete [] TE; // 删除最小生成树的边集
}

template <class VertexType, class EdgeType>
bool adjList<VertexType, EdgeType>::searchEdge(int from, int to) const {
    if(from < 0 || from >= verNum || to < 0 || to >= verNum){
        cout << "Invalid vertex index." << endl;
        return false;
    }
    edgeNode* p = verList[from].firstEdge; // 从from顶点的边表开始查找
    while(p != nullptr && p->to != to){
        p = p->next; // 遍历边表
    }
    if(p == nullptr) return false; // 没有找到边
    return true; // 找到边
}

template <class VertexType, class EdgeType>
bool adjList<VertexType, EdgeType>::insertEdge(int from, int to, EdgeType w){
    if(from < 0 || from >= verNum || to < 0 || to >= verNum){
        cout << "Invalid vertex index." << endl;
        return false;
    }
    edgeNode *p = verList[from].firstEdge, *pre, *s; // 从from顶点的边表开始查找
    while(p != nullptr && p->to != to){
        pre = p; // 记录前一个结点
        p = p->next; // 遍历边表
    }
    if(p != nullptr && p->to == to){
        if(p->weight != w) p->weight = w; // 如果边已存在，更新权重
        else return false;
    }else{
        s = new edgeNode(to, w, p);
        if(p == verList[from].firstEdge)
            verList[from].firstEdge = s; // 插入到边表头部
        else
            pre->next = s; // 在链表其他位置上插入结点
        ++edgeNum; // 新增一条边，边数加一
    }
    return false;
}

template <class VertexType, class EdgeType>
bool adjList<VertexType, EdgeType>::removeEdge(int from, int to){
    if(from < 0 || from >= verNum || to < 0 || to >= verNum){
        cout << "Invalid vertex index." << endl;
        return false;
    }
    edgeNode *p = verList[from].firstEdge, *pre = nullptr;
    while(p != nullptr && p->to < to){
        pre = p;
        p = p->next;
    }
    if((p == nullptr) || (p->to > to))
        return false;
    if(p->to == to){
        if(p == verList[from].firstEdge)
            verList[from].firstEdge = p->next;
        else pre->next = p->next;
        delete p;
        --edgeNum;
        return true;
    }
}

template <class VertexType, class EdgeType>
void adjList<VertexType, EdgeType>::createGraph(const VertexType V[], const EdgeType E[]){
    int i, j;
    for(i = 0; i < verNum; i++)
        verList[i].vertex = V[i];
    for(i = 0; i < verNum; i++)
        for(int j = 0; j < verNum; j++){
            if(E[i * verNum + j] > 0)
                insertEdge(i, j, E[i * verNum + j]); // insertEdge插入边按to值排序
        }
}

template <class VertexType, class EdgeType>
void adjList<VertexType, EdgeType>::printGraph() const {
    int i;
    for(i = 0; i < verNum; i++){
        cout << verList[i].vertex << ":";
        edgeNode *p = verList[i].firstEdge;
        while(p != nullptr){
            cout << verList[p->to].vertex << ' ';
            p = p->next;
        }
        cout << endl;
    }
}

template <class VertexType, class EdgeType>
void adjList<VertexType, EdgeType>::dfs(int start) const {
    visited[start] = true; // 标记当前顶点为已访问
    cout << verList[start].vertex << " "; // 输出当前顶点
    edgeNode* p = verList[start].firstEdge; // 获取当前顶点的边表
    while(p != nullptr){ // 遍历边表
        if(!visited[p->to]){ // 如果边的终点未被访问
            dfs(p->to); // 递归访问
        }
        p = p->next; // 移动到下一条边
    }
}

template <class VertexType, class EdgeType>
void adjList<VertexType, EdgeType>::dfsTraverse() const {
    int i, count = 0;
    for(i = 0; i < verNum; i++)
        visited[i] = false; // 初始化访问标志数组
    for(i = 0; i < verNum; i++){
        if(visited[i] == false){
            dfs(i); // 从未访问的顶点开始深度优先遍历
            count++;
        }
    }
    cout << endl;
}

template <class VertexType, class EdgeType>
void adjList<VertexType, EdgeType>::bfsTraverse() const {
    int v, i, count = 0;
    queue<int> Q;
    edgeNode* p;
    for(i = 0; i < verNum; i++) visited[i] = false; // 初始化访问标志数组
    for(i = 0; i < verNum; i++){
        if(visited[i] == true) continue;
        cout << verList[i].vertex << " "; // 输出当前顶点
        visited[i] = true; // 标记当前顶点为已访问
        Q.push(i); // 将当前顶点入队
        count++;
        while(!Q.empty()){
            v = Q.front(); // 获取队首元素
            Q.pop(); // 出队
            p = verList[v].firstEdge; // 获取当前顶点的边表
            while(p != nullptr){ // 遍历边表
                if(visited[p->to] == false){ // 如果边的终点未被访问
                    cout << verList[p->to].vertex << " "; // 输出边的终点
                    visited[p->to] = true; // 标记为已访问
                    Q.push(p->to); // 将边的终点入队
                }
                p = p->next; // 移动到下一条边
            }
        } 
    }
    cout << endl;
}