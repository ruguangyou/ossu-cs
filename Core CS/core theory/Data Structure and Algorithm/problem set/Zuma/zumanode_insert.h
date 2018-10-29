#pragma once

template <typename T>
ZumaNode<T>* ZumaNode<T>::insertAsPred(T const& e) {
	ZumaNode<T>* x = new ZumaNode(e, pred, this);
	pred->succ = x; pred = x;
	return x;
}

template <typename T>
ZumaNode<T>* ZumaNode<T>::insertAsSucc(T const& e) {
	ZumaNode<T>* x = new ZumaNode(e, this, succ);
	succ->pred = x; succ = x;
	return x;
}