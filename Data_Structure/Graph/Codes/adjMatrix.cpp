#include <iostream>
#include <queue>
#include "Graph.h"
using namespace std;

template <class VertexType, class EdgeType>
class adjMatrix : public Graph<VergexType, EdgeType> {
    VertexType* vertexs; // 顶点向量
    EdgeType** edges; // 邻接矩阵
    EdgeType noEdge; // 无边标志
    void dfs(int start) const; // 从start顶点开始深度优先遍历
public:
    adjMatrix(int size, EdgeType noEdgeFlag);
    ~adjMatrix(); // 析构函数
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
    void floyd() const; // Floyd算法求最短路径
    void printFloyed(EdgeType** D, int** pre) const; // 打印Floyd算法的结果
};

template <class VertexType, class EdgeType>
adjMatrix<VertexType, EdgeType>::adjMatrix(int size, EdgeType noEdgeFlag){
    verNum = size;
    edgeNum = 0;
    noEdge = noEdgeFlag;
    vertex = new VertexType[verNum];
    edges = new EdgeType*[verNum];
    for(int i = 0; i < verNum; i++){
        edges[i] = new EdgeType[verNum];
        for(int j = 0; j < verNum; j++){
            edge[i][j] = noEdge; // 邻接矩阵初始值
        }
    }
    visited = new bool[verNum]; // 访问标志数组
    TE = new mstEdge[verNum - 1]; // 最小生成树的边集
}

template <class VertexType, class EdgeType>
adjMatrix<VertexType, EdgeType>::~adjMatrix(){
    delete [] vertexs; // 删除顶点向量
    for(int i = 0; i < verNum; i++){
        delete [] edges[i];
    }
    delete [] edges; // 删除邻接矩阵
    delete [] visited; // 删除访问标志数组
    delete [] TE; // 删除最小生成树的边集
}

template <class VertexType, class EdgeType>
bool adjMatrix<VertexType, EdgeType>::searchEdge(int from, int to) const {
    if(from < 0 || from >= verNum || to < 0 || to >= verNum) {
        cout << "Invalid vertex index." << endl;
        return false;
    }
    return edges[from][to] != noEdge; // 返回是否存在边
}

template <class VertexType, class EdgeType>
bool adjMatrix<VertexType, EdgeType>::insertEdge(int from, int to, EdgeType w){
    if(from < 0 || from >= verNum || to < 0 || to >= verNum) {
        cout << "Invalid vertex index." << endl;
        return false;
    }
    if(edges[from][to] == noEdge) {
        edges[from][to] = w; // 插入边
        ++edgeNum;
        return true;
    } else {
        cout << "Edge already exists." << endl;
        return false; // 边已存在
    }
}

template <class VertexType, class EdgeType>
bool adjMatrix<VertexType, EdgeType>::removeEdge(int from, int to){
    if(from < 0 || from >= verNum || to < 0 || to >= verNum) {
        cout << "Invalid vertex index." << endl;
        return false;
    }
    if(edges[from][to] != noEdge) {
        edges[from][to] = noEdge; // 删除边
        --edgeNum;
        return true;
    } else {
        cout << "Edge does not exist." << endl;
        return false; // 边不存在
    }
}

template <class VertexType, class EdgeType>
void adjMatrix<VertexType, EdgeType>::createGraph(const VertexType V[], const EdgeType E[]){
    int i, j;
    for(i = 0; i < verNum; i++){
        vertexs[j] = V[i]; // 初始化顶点
    }
    for(i = 0; i < verNum; i++){
        for(j = 0; j < verNum; j++){
            if(E[i * verNum + j] > 0){
                insertEdge(i, j, E[i * verNum + j]); // 插入边
            }
        }
    }
}

template <class VertexType, class EdgeType>
void adjMatrix<VertexType, EdgeType>::printGraph() const {
    int i, j;
    for(i = 0; i < verNum; i++){
        cout << vertexs[i] << ":";
        for(j = 0; j < verNum; j++){
            cout << edges[i][j] << " "; // 打印邻接矩阵
        }
    cout << endl;
    }
}

template <class VertexType, class EdgeType>
void adjMatrix<VertexType, EdgeType>::dfs(int start) const {
    visited[start] = true; // 标记当前顶点为已访问
    cout << vertexs[start] << " "; // 输出当前顶点
    for(int i = 0; i < verNum; i++) {
        if(edges[start][i] != noEdge && visited[i] == false) { // 如果存在边且未访问
            dfs(i); // 递归访问
        }
    }
}

template <class VertexType, class EdgeType>
void adjMatrix<VertexType, EdgeType>::dfsTraverse() const {
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
void adjMatrix<VertexType, EdgeType>::bfsTraverse() const {
    int v, i, j, count = 0;
    queue<int> Q;
    for(i = 0; i < verNum; i++) visited[i] = false; // 初始化访问标志数组
    for(i = 0; i < verNum; i++){
        if(visited[i] == true) continue;
        cout << vertexs[i] << ' ';
        visited[i] = true; // 标记当前顶点为已访问
        Q.push(i); // 将当前顶点入队
        count++;
        while(!Q.empty()){
            v = Q.front(); // 获取队首元素
            Q.pop(); // 出队
            if(visited[v] == false){
                for(j = 0; j < verNum; j++){ // 查找顶点v未被访问的邻接点
                    if(edges[v][j] != noEdge && visited[j] == false){
                        cout << vertexs[j] << ' '; // 输出当前顶点
                        visited[j] = true; // 标记为已访问
                        Q.push(j); // 将顶点入队
                    }
                }
            }
        }
    }
    cout << endl;
}