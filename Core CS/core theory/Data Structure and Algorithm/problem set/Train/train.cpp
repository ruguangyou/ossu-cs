#include <cstdio>

// #include "stack.h"

using namespace std;

#define DEFAULT_CAPACITY 4

template <typename T> class Vector {
protected:
	int _size; int _capacity; T* _elem;

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
		int n = _size++;
		while (n > r) _elem[n] = _elem[--n];
		_elem[r] = e;
	}
	int insert (T const& e) { return insert(_size, e); }

	T remove (int r) {
		_size--;
		while (r < _size) _elem[r] = _elem[++r];
	}
};

template <typename T> class Stack: public Vector<T> {
public:
	Stack (int c = DEFAULT_CAPACITY, int s = 0, T v = 0) { 
		this->_elem = new T[this->_capacity = c];
		for (this->_size = 0; this->_size < s; this->_elem[this->_size++] = v) ;
	}
	void push (T const& e) { this->insert(e); }
	T pop () { return this->remove(this->size() - 1); }
	T& top () { return (*this)[this->size() - 1]; }
};


int main () {
	setvbuf(stdin,  new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);

	int n, m;
	int ia=1, ib=0, ip = 0;

	scanf("%d%d", &n, &m);

	int* B = new int[n];  // int B[n]，当 n 相当大时，会导致"Invalid memory reference"
	Stack<int> S(m);      //  因为该数组创建形式有内存（size）限制，因此用 new 分配内存更加安全.
	int* P = new int[2*n];

	// if (S.empty()) printf("empty\n");
	// S.push(1);
	// if (! S.empty()) printf("not empty\n");
	// S.push(2);
	// if (S.full()) printf("full\n");
	// printf("size: %d\n", S.size());
	// printf("capacity: %d\n", S.capacity());
	// printf("%d %d", S.pop(), S.top());

	for (int i = 0; i < n; i++) scanf("%d", B + i);

	if (m == 0) { printf("No\n"); delete [] B, P; return 0; }

	S.push(ia++); P[ip++] = 1;  // S.push(1), P[1] = 1 (1 represents push, -1 represents pop)
	while (ib < n) {
		while (S.top() != B[ib] && !S.full()) {
			if (ia <= n) { S.push(ia++); P[ip++] = 1; }
			else { printf("No\n"); return 0; }
		}
		if (S.top() != B[ib] && S.full()) { printf("No\n"); delete [] B, P; return 0; }
		S.pop(); ib++; P[ip++] = -1;
	}
	
	for(int i = 0; i < ip ; i++) {
		if (P[i] == 1) printf("push\n");
		if (P[i] == -1) printf("pop\n");
	}

	delete [] B, P; return 0;
}