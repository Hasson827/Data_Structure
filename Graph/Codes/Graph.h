#include <iostream>
using namespace std;

template <class VertexType, class EdgeType>
class Graph{
    struct mstEdge{ // 最小生成树的边结点类型
        int vex1, vex2;
        EdgeType weight; // 边的三元组: (起点,终点, 权重)
        bool operator<(const mstEdge& e) const {
            return weight < e.weight;
        }
    } *TE; // 最小生成树的边集
    int verNum, edgeNum; // 图的顶点数和边数
    bool *visited; // 访问标记数组
public:
    int numOfVertex() const {return verNum;} // 返回图的顶点数
    int numOfEdge() const {return edgeNum;} // 返回图的边数
    virtual void createGraph(const VertexType V[], const EdgeType E[]) = 0; // 创建图
    virtual void dfsTraverse() const = 0; // 深度优先遍历
    virtual void bfsTraverse() const = 0; // 广度优先遍历
    virtual void topSort() const = 0; // 拓扑排序
    virtual void prim(EdgeType noEdge) = 0; // Prim算法求最小生成树
    virtual void kruskal() const = 0; // Kruskal算法求最小生成树
    virtual void printMst() const = 0; // 打印最小生成树
    virtual bool searchEdge(int from, int to) const = 0; // 查找边
    virtual bool insertEdge(int from, int to, EdgeType w) = 0; // 插入权值为w的边
    virtual bool removeEdge(int from, int to) = 0; // 删除边
    virtual void printGraph() const = 0; // 打印图
protected:
    struct mstEdge{
        int vex1, vex2;
        EdgeType weight; // 边的三元组: (起点,终点, 权重)
        bool operator<(const mstEdge& e) const { // 使用优先级队列需要重载<运算符
            return weight < e.weight;
        }
    } *TE; // 最小生成树的边集
    int verNum, edgeNum; // 图的顶点数和边数
    bool *visited; // 访问标记数组
};



