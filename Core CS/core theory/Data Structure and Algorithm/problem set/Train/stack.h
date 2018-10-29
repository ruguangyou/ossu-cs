#pragma once

#include "vector.h"

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