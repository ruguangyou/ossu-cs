#include <cstdio>

using namespace std;

void mergeSort (int* x, int* y, int lo, int hi);
void merge (int* x, int* y, int lo, int mi, int hi);
long long invInside (int* y, int lo, int hi);
long long invBetween (int* y, int lo, int mi, int hi);

#define MAXN 4000000

int* x = new int[MAXN];
int* y = new int[MAXN];

int main() {
	setvbuf(stdin, new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);

	int n;
	long long pairs = 0;

	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d%d", x+i, y+i);

	mergeSort(x, y, 0, n);
	pairs = invInside(y, 0, n);
	printf("%lld\n", pairs);

	return 0;
}

void mergeSort (int* x, int* y, int lo, int hi) {
	if (hi - lo < 2) return;
	int mi = (lo + hi) / 2;

	mergeSort(x, y, lo, mi);
	mergeSort(x, y, mi, hi);
	merge(x, y, lo, mi, hi);

	return;
}

// bug: 错在temp的下标，temp预定义的长度是 mi-lo，之前用temp[i]，i从 lo 开始，但应该用temp[i-lo]
void merge (int* x, int* y, int lo, int mi, int hi) {
	int* temp1 = new int[mi-lo];
	int* temp2 = new int[mi-lo];
	for (int i = lo; i < mi; i++) { temp1[i-lo] = x[i]; temp2[i-lo] = y[i]; }

	for (int i=lo, j=mi, k=lo; k < hi; ) {
		if (i < mi && (j >= hi || temp1[i-lo] <= x[j])) { y[k] = temp2[i-lo]; x[k++] = temp1[i-lo]; i++; }
		if (j < hi && (i >= mi || temp1[i-lo] >  x[j])) { y[k] = y[j]; x[k++] = x[j++]; }
	}

	delete [] temp1;
	delete [] temp2;
	return;
}

long long invInside (int* y, int lo, int hi) {
	if (hi - lo < 2) return 0;
	int mi = (lo + hi) / 2;

	return invInside(y, lo, mi) + invInside(y, mi, hi) + invBetween(y, lo, mi, hi);
}

long long invBetween (int* y, int lo, int mi, int hi) {
	long long pairs = 0;
	int* temp = new int[mi-lo];
	for (int i = lo; i < mi; temp[i-lo] = y[i++]) ;

	for (int i=lo, j=mi, k=lo; k < hi; ) {
		if (i < mi && (j >= hi || temp[i-lo] <  y[j])) { y[k++] = temp[i-lo]; i++; pairs += hi-j; }
		if (j < hi && (i >= mi || temp[i-lo] >= y[j])) y[k++] = y[j++];
	}

	delete [] temp;
	return pairs;
}