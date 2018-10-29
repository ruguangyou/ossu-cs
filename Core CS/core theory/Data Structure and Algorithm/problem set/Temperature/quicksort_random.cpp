#include <cstdio>
#include <cstdlib>  // using rand() to randomize the selection of pivot

void swap (int* x, int* y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

void quicksort (int* p, int start, int end) {
	if (start >= end) return;
	int left = start;
	int right = end - 1;

	// randomize
	int index = rand() % (end + 1 - start) + start;
	if (index < end) swap(&p[index], &p[end]);
	
	int pivot = p[end];
	while (left < right) {
		while (p[left] < pivot && left < right)
			left++;
		while (p[right] >= pivot && left < right)
			right--;
		swap(&p[left], &p[right]);
	}
	if (p[left] >= pivot)
		swap(&p[left], &p[end]);
	else
		left++;
	quicksort(p, start, left-1);
	quicksort(p, right+1, end);
}


int main() {
	int n;
	int p[100];
	printf("How many numbers to be sorted: ");
	scanf("%d", &n);
	printf("\nInputs: ");
	for (int i = 0; i < n; i++) scanf("%d", p+i);
	
	quicksort(p, 0, n-1);
	printf("\nOutputs: ");
	for (int i = 0; i < n; i++) printf("%d ", p[i]);
	printf("\n");
}