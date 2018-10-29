#pragma once

template <typename T> ZumaNode<T>* Zuma<T>::remove(ZumaNode<T>* p) {
	ZumaNode<T>* q = new ZumaNode<T>();
	q = p->succ;
	q->pred = p->pred;
	p->pred->succ = q;
	delete p;
	_size--;
	if (_size == 0) return NULL;
	if (q == trailer) return last();
	return q;
}