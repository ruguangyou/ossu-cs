#include <cstdio>

static int mid;

void swap (int* x, int* y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

int findMedian (int* p, int start, int end) {
	if (start == end)  // mid is in [start, end], if start == end, then mid == start == end
		return p[mid];
	
	int left = start;
	int right = end - 1;
	// if (p[right] < p[end])
	// 	swap(&p[right], &p[end]);
	int pivot = p[end];

	while (left < right) {
		while (left < right && p[left] < pivot)
			left++;
		while (left < right && p[right] >= pivot)
			right--;
		swap(&p[left], &p[right]);
	}
	if (p[left] >= pivot) 
		swap(&p[left], &p[end]);
	else
		swap(&p[++left], &p[end]);
	if (left < mid) findMedian(p, left+1, end);
	else if (left > mid) findMedian(p, start, left-1);
	else return p[mid];

	// if (p[left] >= pivot) {
	// 	swap(&p[left], &p[end]);
	// 	if (left < mid) findMedian(p, left, end);
	// 	else if (left > mid) findMedian(p, start, right);
	// 	else return p[mid];
	// }
	// else {
	// 	if (end == mid) return p[mid];
	// 	else return findMedian(p, start, end-1);
	// }
}

int main() {
	int n;
	int p[100];
	printf("How many numbers: ");
	scanf("%d", &n);
	mid = (n - 1) / 2;
	printf("\nInputs: ");
	for (int i = 0; i < n; i++) scanf("%d", p+i);
	
	printf("\nOutputs: %d\n", findMedian(p, 0, n-1));
}