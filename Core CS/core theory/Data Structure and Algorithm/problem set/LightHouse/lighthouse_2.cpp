#include <cstdio>

#define MAX_INPUT 4000000

void mergeSort (int* v, int* w, int lo, int hi);
void merge(int* v, int* w, int lo, int mi, int hi);
void invInside(int* v, int lo, int hi);
void invBetween(int* v, int lo, int mi, int hi);

int* xs = new int[MAX_INPUT];
int* ys = new int[MAX_INPUT];
long long pairs = 0;

int main() {
	int n;

	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d%d", xs+i, ys+i);

	mergeSort(xs, ys, 0, n);
	
	invInside(ys, 0, n);
	printf("%lld\n", pairs);

	//delete [] xs; delete [] ys;
	return 0;
}

void mergeSort (int* v, int* w, int lo, int hi) {
	if (hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	mergeSort(v, w, lo, mi);
	mergeSort(v, w, mi, hi);
	merge(v, w, lo, mi, hi);
}

void merge(int* v, int* w, int lo, int mi, int hi) {
	int* temp = new int[mi-lo];
	int* temp2 = new int[mi-lo];
	for (int i = lo; i < mi; i++) { temp[i] = v[i]; temp2[i] = w[i]; }
	for (int i = lo, j = mi, k = lo; k < hi; ) {
		if (i < mi && (temp[i] <  v[j] || j > hi-1)) { w[k] = temp2[i]; v[k++] = temp[i++];}
		if (j < hi && (temp[i] >= v[j] || i > mi-1)) { w[k] = w[j]; v[k++] = v[j++]; }
	}
	//delete [] temp;
	//delete [] temp2;
	return;
}

void invInside(int* v, int lo, int hi) {
	if (hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	invInside(v, lo, mi);
	invInside(v, mi, hi);
	invBetween(v, lo, mi, hi);
	printf("inside: %lld [%d,%d)\n", pairs, lo, hi);
	return;
}

void invBetween(int* v, int lo, int mi, int hi) {
	int* temp = new int[mi-lo];
	for (int i = lo; i < mi; temp[i] = v[i++]);
	for (int i = lo, j = mi, k = lo; k < hi; ) {
		if (i < mi && (temp[i] <  v[j] || j > hi-1)) { v[k++] = temp[i++]; pairs += hi-j; }
		if (j < hi && (temp[i] >= v[j] || i > mi-1)) v[k++] = v[j++];
	}
	//delete [] temp;
	printf("between: %lld [%d,%d) [%d,%d)\n", pairs, lo, mi, mi, hi);
	return;
}