#include <cstdio>

using namespace std;

#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p->height) : -1)
#define DEFAULT_CAPACITY 4

template <typename T> class Vector {
protected:
	int _size; int _capacity; T* _elem;

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
		int n = _size++;
		while (n > r) _elem[n] = _elem[--n];
		_elem[r] = e;
	}
	int insert (T const& e) { return insert(_size, e); }

	T remove (int r) {
		T e = _elem[r];
		_size--;
		while (r < _size) _elem[r] = _elem[++r];
		return e;
	}
};

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

template <typename T> struct BinNode {
	// member
	T data;
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;
	int height;
	// constructor
	BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0) {}
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int h = 0):
		data(e), parent(p), lc(lc), rc(rc), height(h) {}
	// interface
	int size() {  // 以当前节点为根的树的节点总数
		int s = 1;
		if (lc) s += lc->size();
		if (rc) s += rc->size();
		return s;
	}
	BinNodePosi(T) insertAsLc (T const& e) { return lc = new BinNode(e, this); }  // 似乎是抛弃了原有的子节点
	BinNodePosi(T) insertAsRc (T const& e) { return rc = new BinNode(e, this); }

	template <typename VST> void travIn (VST& visit) {
		Stack<BinNodePosi(T)> S(size());
		BinNodePosi(T) temp = this;
		while (true) {
			while (temp) { S.push(temp); temp = temp->lc; }
			if (S.empty()) return;
			temp = S.pop(); visit(temp->data);
			temp = temp->rc;
		}
		// do {
		// 	while (temp->lc) { S.push(temp); temp = temp->lc; }
		// 	visit(temp->data);
		// 	while (!temp->rc) { if (S.empty()) return; temp = S.pop(); visit(temp->data); }
		// 	temp = temp->rc;
		// } while (true);
	}
};

template <typename T> class BinTree {
protected:
	int _size; BinNodePosi(T) _root;
	virtual int updateHeight (BinNodePosi(T) x) { 
		return x->height = 1 + (stature(x->lc) > stature(x->rc) ? stature(x->lc) : stature(x->rc)); 
	}
	void updateHeightAbove (BinNodePosi(T) x) { while (x) { updateHeight(x); x = x->parent; } }
public:
	BinTree() : _size(0), _root(NULL) {}
	// ~BinTree() { if (0 < _size) remove(_root); }
	int size() { return _size; }
	bool empty() { return !_root; }
	BinNodePosi(T) root() { return _root; }
	BinNodePosi(T) insertAsRoot (T const& e) { _size = 1; return _root = new BinNode<T>(e); }  // 插入空的二叉树
	BinNodePosi(T) insertAsLc (BinNodePosi(T) x, T const& e) {
		_size++; x->insertAsLc(e); updateHeightAbove(x); return x->lc;
	}
	BinNodePosi(T) insertAsRc (BinNodePosi(T) x, T const& e) {
		_size++; x->insertAsRc(e); updateHeightAbove(x); return x->rc;
	}
	// int remove (BinNodePosi(T) x);
	template <typename VST> void travIn (VST& visit) { if (_root) _root->travIn(visit); }
};

BinTree<int> bt;

template <typename T> void print (T const& e) {
	printf("%d ", e);
}

template <typename T> void rebuild(BinNodePosi(T) x, int *pre, int* post, int n) {
	if (n == 1) return;
	int r = 0;
	while (post[r++] != pre[1]) ;
	
	rebuild(bt.insertAsLc(x, pre[1]),  pre+1, post, r);
	rebuild(bt.insertAsRc(x, pre[r+1]), pre+r+1, post+r, n-r-1);
}


int main() {
	setvbuf(stdin,  new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);
	BinNodePosi(int) x;
	int n, r;
	scanf("%d", &n);

	int* pre = new int[n];
	int* post = new int[n];

	for (int i = 0; i < n; i++) scanf("%d", pre + i);
	for (int i = 0; i < n; i++) scanf("%d", post + i);

	x = bt.insertAsRoot(pre[0]);
	rebuild(x, pre, post, n);

	bt.travIn(print<int>);
	delete [] pre, post;
	return 0;
}