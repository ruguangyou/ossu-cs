#pragma once

template <typename T> ZumaNode<T>* Zuma<T>::operator[] (Rank r) {
	ZumaNode<T>* p = header->succ;
	while(0 < r--) p = p->succ;
	return p;
}