
#include "zumanode.h"

typedef int Rank;

template <typename T> class Zuma {
private:
	int _size;
	ZumaNode<T>* header; ZumaNode<T>* trailer;  // 头哨兵，尾哨兵

protected:
	void init();  // link header and trailer, set _size = 0
	int clear();  // return oldSize

public:
	Zuma() { init(); }
	~Zuma();

	int size() const { return _size; }
	ZumaNode<T>* first() { return header->succ; }
	ZumaNode<T>* last() { return trailer->pred; }
	ZumaNode<T>* operator[] (Rank r);
	ZumaNode<T>* insertAsFirst(T const& e);
	ZumaNode<T>* insertAsLast(T const& e);
	ZumaNode<T>* insertPred(ZumaNode<T>* p, T const& e);
	ZumaNode<T>* insertSucc(ZumaNode<T>* p, T const& e);
	ZumaNode<T>* sameColor(ZumaNode<T>* p);
	ZumaNode<T>* remove(ZumaNode<T>* p);
	ZumaNode<T>* removeSameColor(ZumaNode<T>* p);
};

#include "zuma_implementation.h"