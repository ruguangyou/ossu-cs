#pragma once

#include "queue.h"
#include "stack.h"

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;  // 顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;  // 边在遍历树中的类型

template <typename Tv, typename Te> class Graph {
private:
	void reset() {
		for (int i = 0; i < n; i++) {
			status(i) = UNDISCOVERED; dtime(i) = ftime(i) = -1;
			parent(i) = -1;
			//priority(i) = INT_MAX;
			for (int j = 0; j < n; j++) if (exists(i, j)) type(i, j ) = UNDETERMINED;
		}
	}
	void BFS (int v, int& clock) {
		Queue<int> Q;
		status(v) = DISCOVERED; Q.enqueue(v);
		while (!Q.empty()) {
			int v = Q.dequeue(); dtime(v) = ++clock;
			for (int u = firstNbr(v); -1 < u ; u = nextNbr(v, u)) {
				if (status(u) == UNDISCOVERED) {
					status(u) = DISCOVERED; Q.enqueue(u);
					type(v, u) = TREE; parent(u) = v;
				}
				else {
					type(v, u) = CROSS;
				}
			}
			status(v) = VISITED;
		}
	}
	void DFS (int v, int& clock) {
		status(v) = DISCOVERED; dtime(v) = ++clock;
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
			switch (status(u)) {
				case UNDISCOVERED:
					type(v, u) = TREE; parent(u) = v; DFS(u, clock); break;
				case DISCOVERED:
					type(v, u) = BACKWARD; break;
				default:
					type(v, u) = (dtime(v) < dtime(u) ? FORWARD : CROSS); break;
			}
		}
	}

public:
// virtual ... = 0; 纯虚函数，= 0 告诉编译器函数没有主体
	// 顶点，<key, data> pair，约定通过 key (int) 来访问 vertex
	int n;  // 顶点数
	virtual int insert (Tv const& ) = 0;  // 插入顶点，返回编号
	virtual Tv remove (int) = 0;  // 删除顶点及其关联边，返回该顶点信息
	virtual Tv& vertex (int) = 0;  // 顶点v的数据
	virtual int inDegree (int) = 0;  // 顶点v的入度
	virtual int outDegree (int) = 0; // 顶点v的出度
	virtual int firstNbr (int) = 0;
	virtual int nextNbr (int, int) = 0;
	virtual VStatus& status (int) = 0;
	virtual int& dtime (int) = 0;
	virtual int& ftime (int) = 0;
	virtual int& parent (int) = 0;  // 顶点v在遍历树中的父节点
	virtual int& priority (int) = 0;  // 顶点v在遍历树中优先级
	// 边，此处约定边有向，即无向边处理为一对有向边
	int e;  // 边数
	virtual bool exists (int, int) = 0;
	virtual void insert (Te const&, int, int, int) = 0;  // 在顶点v和u之间插入一条权重为w的边e
	virtual Te remove (int, int) = 0;
	virtual Te& edge (int, int) = 0;
	virtual EType& type (int, int) = 0;
	virtual int& weight (int, int) = 0;
	// 算法
	void bfs (int v) {
		reset(); int clock = 0; int u = v;
		do
			if (status(u) == UNDISCOVERED) BFS(u, clock);
		while (v != ( u = (++u % n) ));
	}  // 广度优先搜索
	void dfs (int v) {
		reset(); int clock = 0; int u = v;
		do
			if (status(u) == UNDISCOVERED) DFS(u, clock);
		while (v != ( u = (++u % n) ));
	}  // 深度优先搜索
};

template <typename Tv> struct Vertex {
	Tv data; int inDegree, outDegree;
	VStatus status; int dtime, ftime;
	int parent; int priority;
	Vertex (Tv const& d = (Tv) 0): data(d), inDegree(0), outDegree(0),
	status(UNDISCOVERED), dtime(-1), ftime(-1), parent(-1), priority(0) {}
};

template <typename Te> struct Edge {
	Te data; int weight; EType type;
	Edge (Te const& d, int w): data(d), weight(w), type(UNDETERMINED) {}
};