#pragma once

template <typename T> ZumaNode<T>* Zuma<T>::removeSameColor(ZumaNode<T>* p) {
	T e = p->color;
	while( p != NULL && p->color == e) p = remove(p);
	return p;
}