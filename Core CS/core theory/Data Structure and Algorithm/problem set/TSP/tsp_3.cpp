#include <cstdio>
// #include "vector.h"
using namespace std;

#define DEFAULT_CAPACITY 10

template <typename T> class Vector {
protected:
	int _size; int _capacity; T* _elem;
	void expand() {
		if (_size < _capacity) return;
		if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
		T* oldElem = _elem; _elem = new T[_capacity <<= 1];
		for (int i = 0; i < _size; i++)
			_elem[i] = oldElem[i];
		delete [] oldElem;
	}

public:
	Vector (int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v) ;
	}

	~Vector() { delete [] _elem; }

	int size() { return _size; }
	int capacity() { return _capacity; }

	bool empty() { return !_size; }
	bool full() { return _size == _capacity; }

	T& operator [] (int r) const { return _elem[r]; }

	int insert (int r, T const& e) {
		expand();
		int n = _size++;
		while (n > r) _elem[n] = _elem[--n];
		_elem[r] = e;
		return r;
	}
	int insert (T const& e) { return insert(_size, e); }

	T remove (int r) {
		T e = _elem[r];
		_size--;
		while (r < _size) _elem[r] = _elem[++r];
		return e;
	}
};



typedef enum { UNDISCOVERED, DISCOVERED } VStatus;

struct Node {
	int data; int depth;
	Node* next;
	VStatus status;
	Node(int d = 0) : data(d), depth(0), next(NULL), status(UNDISCOVERED) {}
};

int DFS(Vector<Node*> const& V, int j) {
	int local_max = 1; int temp;
	if (!V[j]) return local_max;
	V[j]->status = DISCOVERED;
	Node* p = V[j]->next;
	while (p) {
		int d = p->data;
		if (V[d-1] && V[d-1]->status == DISCOVERED) temp = 1 + V[d-1]->depth;
		else temp = 1 + DFS(V, d-1);
		if (local_max < temp) local_max = temp;
		p = p->next;
	}
	V[j]->depth = local_max;
	return local_max;
}


int main() {
	setvbuf(stdin,  new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);

	int n, m; int u, v; int max = 1, temp;

	scanf("%d%d", &n, &m);
	Vector<Node*> V(n, n, NULL);

	for (int i = 0; i < m; i++) {
		scanf("%d%d", &u, &v);
		if (!V[u-1]) { V[u-1] = new Node(u); V[u-1]->next = new Node(v); }
		else { Node* temp = new Node(v); temp->next = V[u-1]->next; V[u-1]->next = temp; }
	}

	for (int j = 0; j < n; j++) {
		if (V[j] && V[j]->status == UNDISCOVERED) { 
			temp = DFS(V, j);
			if (max < temp) max = temp;
		}
	}
	printf("%d\n", max);
	
	return 0;
}