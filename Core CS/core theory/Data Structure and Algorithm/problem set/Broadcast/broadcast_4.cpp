#include <cstdio>
// #include "vector.h"

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

struct Town {
	int data, fm; bool visited;
	Town* next;
	Town (int d = -1) : data(d), fm(0), visited(false), next(NULL) {}
};

// void swap (int& u, int& v) { int temp = u; u = v; v = temp; }

int main() {
	int n, m; int u, v;
	scanf("%d%d", &n, &m);
	Vector<Town*> T(n, n, NULL);
	int* queue = new int[2*m]; int head = 0, tail = 0;

	for (int i = 0; i < m; i++) {
		scanf("%d%d", &u, &v); u--; v--;
		// if (u > v) swap(u, v);
		if (!T[u]) T[u] = new Town(u);
		if (u == v) continue;
		if (!T[v]) T[v] = new Town(v);

		Town* p = new Town(v);
		p->next = T[u]->next; T[u]->next = p;
		Town* q = new Town(u);
		q->next = T[v]->next; T[v]->next = q;

	}

	for (int j = 0; j < n; j++) {
		if (!T[j] || T[j]->visited) continue;
		T[j]->fm = 1;
		queue[tail++] = T[j]->data;
		while (head < tail) {
			Town* p = T[queue[head++]]; p->visited = true;
			int f = p->fm;
			while (p = p->next) {
				int d = p->data;
				if (T[d]->fm == f) { printf("-1\n"); return 0; }
				T[d]->fm = -f;
				if (!T[d]->visited) queue[tail++] = d;
			}
		}
	}
	printf("1\n"); return 0;
}