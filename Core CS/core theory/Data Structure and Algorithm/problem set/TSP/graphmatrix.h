#pragma once

#include "graph.h"

template <typename Tv, typename Te> class GraphMatrix : public Graph<Tv, Te> {
private:
	Vector< Vertex<Tv> > V;
	Vector< Vector< Edge<Te>* > > E;
public:
	GraphMatrix() { this->n = this->e = 0; }
	~GraphMatrix() {
		for (int i = 0; i < this->n; i++)
			for (int j = 0; j < this->n; j++)
				delete E[i][j];
	}

	virtual Tv& vertex (int i) { return V[i].data; }
	virtual int inDegree (int i) { return V[i].inDegree; }
	virtual int outDegree (int i) { return V[i].outDegree; }
	virtual int firstNbr (int i) { return nextNbr(i, this->n); }
	virtual int nextNbr (int i, int j) { while ( (-1 < j) && ( !exists(i, --j) ) ); return j; }
	virtual VStatus& status (int i) { return V[i].status; }
	virtual int& dtime (int i) { return V[i].dtime; }
	virtual int& ftime (int i) { return V[i].ftime; }
	virtual int& parent (int i) { return V[i].parent; }
	virtual int& priority (int i) { return V[i].priority; }

	virtual int insert (Tv const& d) {
		for (int j = 0; j < this->n; j++) E[j].insert(NULL);
		this->n++;
		E.insert(Vector< Edge<Te>* >(this->n, this->n, (Edge<Te>*) NULL));
		return V.insert(Vertex<Tv>(d));
	}
	virtual Tv remove (int i) {
		for (int j = 0; j < this->n; j++)
			if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; }
		E.remove(i); this->n--;
		Tv vBak = vertex(i); V.remove(i);
		for (int j = 0; j < this->n; j++)
			if (Edge<Te>* e = E[j].remove(i)) {delete e; V[j].outDegree--; }
		return vBak;
	}

	virtual bool exists (int i, int j) {
		return (0 <= i) && (i < this->n) && (0 <= j) && (j < this->n) && E[i][j] != NULL;
	}
	virtual EType& type (int i, int j) { return E[i][j]->type; }
	virtual Te& edge (int i, int j) { return E[i][j]->data; }
	virtual int& weight (int i, int j) { return E[i][j]->weight; }

	virtual void insert (Te const& edge, int w, int i, int j) {
		if (exists(i, j)) return;
		E[i][j] = new Edge<Te>(edge, w);
		this->e++; V[i].outDegree++; V[j].inDegree++;
	}
	virtual Te remove (int i, int j) {
		Te eBak = edge(i, j); delete E[i][j]; E[i][j] = NULL;
		this->e--; V[i].outDegree--; V[j].inDegree--;
		return eBak;
	}
};