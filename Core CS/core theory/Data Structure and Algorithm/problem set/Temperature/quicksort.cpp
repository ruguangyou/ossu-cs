#include <cstdio>

void quicksort (int* p, int start, int end) {  // [start, end]
	if (start >= end) return;
	int left = start; int right = end;
	int pivot = p[right];
	bool front = true;
	while (left < right) {
		if (!front) {
			if (p[right] <= pivot) { p[left++] = p[right]; front = !front; }
			else right--;
			continue;
		}
		else {
			if (p[left] > pivot) { p[right--] = p[left]; front = !front; }
			else left++;
		}
	}
	p[right] = pivot;

	quicksort(p, start, right-1);  // [start, ritht-1], p[right] is where pivot locates and is in its correct order, no need to be sorted further
	quicksort(p, left+1, end);  // [left+1, end]
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