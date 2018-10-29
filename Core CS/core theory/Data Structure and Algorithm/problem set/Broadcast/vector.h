#pragma once

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