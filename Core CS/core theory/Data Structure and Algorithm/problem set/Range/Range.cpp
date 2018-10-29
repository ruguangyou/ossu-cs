#include <cstdio>
// #include "Range_binSearch.h"
// #include "Range_mergeSort.h"
// #include "Range_merge.h"

using namespace std;

typedef int Rank;
/* Let S be a set of n interger points on the x-axis.
   For each given interval [a, b], count the points lying inside. */

Rank binSearch(Rank lo, Rank hi, int* R, int const& e) {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		(e < R[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}

void merge(Rank lo, Rank mi, Rank hi, int* R) {
	int* A = R + lo;
	int lb = mi-lo; int* B = new int[lb];
	for (Rank i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi; int* C = R + mi;
	/*
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) {
		if ( (j < lb) && ( (k >= lc) || (B[j] <= C[k]) ) ) A[i++] = B[j++];
		if ( (k < lc) && ( (j >= lb) || (B[j] >  C[k]) ) ) A[i++] = C[k++];
	}
	*/
	for (Rank i = 0, j = 0, k = 0; (j < lb); ) {
		if ( (k >= lc) || (B[j] <= C[k]) ) A[i++] = B[j++];
		if ( (k <  lc) && (B[j] >  C[k]) ) A[i++] = C[k++];
	}

	delete [] B;
}

void mergeSort(Rank lo, Rank hi, int* R) {
	if (hi - lo < 2) return;
	Rank mi = (lo + hi) >> 1;
	mergeSort(lo, mi, R);
	mergeSort(mi, hi, R);
	merge(lo, mi, hi, R);
}


int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n, m; // n is the size of S, m is the number of queries
	int a, b; 

	//printf("first line input: %d\n", n);
	scanf("%d%d", &n, &m);
	int R[n];

	// printf("second line input: ");
	for (int i=0; i<n; ++i) { scanf("%d", R+i); }
	mergeSort(0, n, R);
	//for (Rank i = 0; i < n; printf("%d ", R[i++]));

	//printf("rest input: \n");
	for (int j=0; j<m; ++j) {
		scanf("%d%d", &a, &b);
		printf("%d\n", binSearch(0, n, R, b) - binSearch(0, n, R, a-1));
	}
	return 0;
}