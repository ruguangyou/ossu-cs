#include "Range_merge.h"

void mergeSort(int lo, int hi, int* R) {
	if (hi - lo < 2) return;
	int mi = (lo + hi) >> 1;
	mergeSort(lo, mi, R);
	mergeSort(mi, hi, R);
	merge(lo, mi, hi, R);
}