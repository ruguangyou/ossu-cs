#include <cstdio>

using namespace std;

void mergeSort (int* x, int* y, int lo, int hi);
void merge (int* x, int* y, int lo, int mi, int hi);
long long invInside (int* y, int lo, int hi);
long long invBetween (int* y, int lo, int mi, int hi);

#define MAXN 4000000

int* x = new int[MAXN];
int* y = new int[MAXN];

// 改进： merge 和 invBetween 内 new、delete temp改成全局变量，
//       从而节省每次调用函数时 new、delete 的开销
//       此外，测试结果显示，原先版本里若不进行 delete 能省下一定时间，
//       但代价是内存使用量非常大 (memory limit exceeded)
int* temp_x = new int[MAXN];
int* temp_y = new int[MAXN];

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

void merge (int* x, int* y, int lo, int mi, int hi) {
	for (int i = lo; i < mi; i++) { temp_x[i] = x[i]; temp_y[i] = y[i]; }

	for (int i=lo, j=mi, k=lo; k < hi; ) {
		if (i < mi && (j >= hi || temp_x[i] <= x[j])) { y[k] = temp_y[i]; x[k++] = temp_x[i++]; }
		else if (j < hi && (i >= mi || temp_x[i] > x[j])) { y[k] = y[j]; x[k++] = x[j++]; }
	}

	return;
}

long long invInside (int* y, int lo, int hi) {
	if (hi - lo < 2) return 0;
	int mi = (lo + hi) / 2;

	return invInside(y, lo, mi) + invInside(y, mi, hi) + invBetween(y, lo, mi, hi);
}

long long invBetween (int* y, int lo, int mi, int hi) {
	long long pairs = 0;
	for (int i = lo; i < mi; temp_y[i] = y[i++]) ;

	for (int i=lo, j=mi, k=lo; k < hi; ) {
		if (i < mi && (j >= hi || temp_y[i] < y[j])) { y[k++] = temp_y[i++]; pairs += hi-j; }
		else if (j < hi && (i >= mi || temp_y[i] >= y[j])) y[k++] = y[j++];
	}

	return pairs;
}