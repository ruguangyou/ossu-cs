#pragma once

template <typename T> void Zuma<T>::init() {
	_size = 0;
	header = new ZumaNode<T>;
	trailer = new ZumaNode<T>;
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
}