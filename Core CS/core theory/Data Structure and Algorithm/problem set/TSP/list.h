#pragma once

typedef int Rank;

template <typename T>
struct ListNode {
	T color;
	ListNode<T>* pred;  // predessor
	ListNode<T>* succ;  // successor

	ListNode() {}  // 针对header和trailer的构造
	ListNode(T e, ListNode<T>* p = NULL, ListNode<T>* s = NULL) : color(e), pred(p), succ(s) {}  // 默认构造器

	ListNode<T>* insertAsPred(T const& e) {
		ListNode<T>* x = new ListNode(e, pred, this);
		pred->succ = x; pred = x;
		return x;
	}  // 紧靠当前节点之前插入新节点
	ListNode<T>* insertAsSucc(T const& e) {
		ListNode<T>* x = new ListNode(e, this, succ);
		succ->pred = x; succ = x;
		return x;
	}  // 紧靠当前节点之后插入新节点
};


template <typename T> class List {
private:
	int _size;
	ListNode<T>* header; ListNode<T>* trailer;  // 头哨兵，尾哨兵

protected:
	void init() {
		_size = 0;
		header = new ListNode<T>;
		trailer = new ListNode<T>;
		header->succ = trailer; header->pred = NULL;
		trailer->pred = header; trailer->succ = NULL;
	}  // link header and trailer, set _size = 0
	int clear() {
		int oldSize = _size;
		while( 0 < _size) remove(header->succ);
		return oldSize;
	}  // return oldSize

public:
	List() { init(); }
	~List();

	int size() const { return _size; }
	bool empty() const { return _size <= 0; }
	ListNode<T>* first() { return header->succ; }
	ListNode<T>* last() { return trailer->pred; }
	T& operator[] (Rank r) {
		ListNode<T>* p = header->succ;
		while(0 < r--) p = p->succ;
		return p->data;
	}
	ListNode<T>* insertAsFirst(T const& e) { _size++; return header->insertAsSucc(e); }
	ListNode<T>* insertAsLast(T const& e) { _size++; return trailer->insertAsPred(e); }
	ListNode<T>* insertPred(ListNode<T>* p, T const& e) { _size++; return p->insertAsPred(e); }
	ListNode<T>* insertSucc(ListNode<T>* p, T const& e) { _size++; return p->insertAsSucc(e); }
	T remove(ListNode<T>* p) {
		T e = p->data;
    	p->pred->succ = p->succ; p->succ->pred = p->pred;
    	delete p; _size--;
    	return e;
	}
};