#include <cstdio>
#include "Range_mergeSort.h"
#include "Range_binSearch.h"

int main() {
	int a = 4, b = 55;
	int R[10] = {1,4,23,2,6,3,9,4,54,7};
	mergeSort(0,10,R);
	for (int i=0; i<10; printf("%d ", R[i++]));

	printf("# points lying inside [%d, %d]: %d\n", a, b, binSearch(0, 10, R, b)-binSearch(0, 10, R, a-1));
	return 0;
}