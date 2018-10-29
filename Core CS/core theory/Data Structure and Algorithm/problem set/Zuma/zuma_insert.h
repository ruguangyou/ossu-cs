#pragma once

template <typename T> ZumaNode<T>* Zuma<T>::insertAsFirst(T const& e) {
	_size++; return header->insertAsSucc(e);
}

template <typename T> ZumaNode<T>* Zuma<T>::insertAsLast(T const& e) {
	_size++; return trailer->insertAsPred(e);
}

template <typename T> ZumaNode<T>* Zuma<T>::insertPred(ZumaNode<T>* p, T const& e) {
	_size++; return p->insertAsPred(e);
}

template <typename T> ZumaNode<T>* Zuma<T>::insertSucc(ZumaNode<T>* p, T const& e) {
	_size++; return p->insertAsSucc(e);
}