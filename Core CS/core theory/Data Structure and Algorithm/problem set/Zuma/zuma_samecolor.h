#pragma once

template <typename T> ZumaNode<T>* Zuma<T>::sameColor(ZumaNode<T>* p) {
	if (p == NULL) return NULL;
	T e = p->color;
	while (p != first() && p->pred->color == e) p = p->pred;
	if (p != last() && p->succ != last() && p->succ->succ->color == e) return p;
	else return NULL;
}