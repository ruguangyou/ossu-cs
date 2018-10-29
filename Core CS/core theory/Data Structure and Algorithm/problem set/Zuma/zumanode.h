#pragma once

template <typename T>
struct ZumaNode {
	T color;
	ZumaNode<T>* pred;  // predessor
	ZumaNode<T>* succ;  // successor

	ZumaNode() {}  // 针对header和trailer的构造
	ZumaNode(T e, ZumaNode<T>* p = NULL, ZumaNode<T>* s = NULL) : color(e), pred(p), succ(s) {}  // 默认构造器

	ZumaNode<T>* insertAsPred(T const& e);  // 紧靠当前节点之前插入新节点
	ZumaNode<T>* insertAsSucc(T const& e);  // 紧靠当前节点之后插入新节点
};

#include "zumanode_insert.h"