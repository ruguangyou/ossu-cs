// #include "temperature_lib.c"
#include <cstdio>
#include "temperature.h"

using namespace std;

typedef struct _station_type
{
    int x, y;
    int temp;
} station_type;

static FILE * fr = NULL;
static FILE * fw = NULL;

static int n, m;
static int last_response, query_index;
static station_type stations[50000];

static int mid;

template <typename T> void swap (T* s1, T* s2) {
	int a;
	a = s1->x; s1->x = s2->x; s2->x = a;
	a = s1->y; s1->y = s2->y; s2->y = a;
	a = s1->temp; s1->temp = s2->temp; s2->temp = a;
}

template <typename T> struct KdTree {
	T data;  // records the data of point that lies in the split line
	int splitDirection;  // 0 represents x-axis, 1 represents y-axis
	int splitLine;  // records the median of selected split direction
	KdTree<T>* lc;
	KdTree<T>* rc;

	KdTree<T> () : lc(NULL), rc(NULL) {}
};

template <typename T> KdTree<T>* buildKdTree (T* P, int d, int size) {
	if (size == 0) return NULL;

	if (size == 1) {
		KdTree<T>* leaf = new KdTree<T>();
		leaf->data = P[0];
		return leaf;
	}
	KdTree<T>* root = new KdTree<T>();

	root->splitDirection = d % 2;
	mid = (size - 1) / 2;
	root->splitLine = findMedian(root->splitDirection, P, 0, size-1);
	T* LP = new T[size/2+1]; int Lsize = 0;
	T* RP = new T[size/2+1]; int Rsize = 0;

	if (d%2) {  // y-axis
		for (int i = 0; i < size; i++) {
			if (P[i].y <= root->splitLine) LP[Lsize++] = P[i];
			else RP[Rsize++] = P[i];
		}
	}
	else {  // x-axis
		for (int i = 0; i < size; i++) {
			if (P[i].x <= root->splitLine) LP[Lsize++] = P[i];
			else RP[Rsize++] = P[i];
		}
	}

	root->lc = buildKdTree(LP, d+1, Lsize);
	root->rc = buildKdTree(RP, d+1, Rsize);
	return root;
}

template <typename T> int findMedian (int direction, T* P, int start, int end) {
	int left = start;
	int right = end - 1;

	if (direction) {  // direction != 0, means y-axis
		if (start == end) return P[mid].y;  // mid is in [start, end], if start == end, then mid == start == end

		// if (P[right].y < P[end].y) swap(&P[right], &P[end]);
		int pivot = P[end].y;

		while (left < right) {
			while (left < right && P[left].y < pivot)
				left++;
			while (left < right && P[right].y >= pivot)
				right--;
			swap(&P[left], &P[right]);
		}
		if (P[left].y >= pivot) swap(&P[left], &P[end]);
		else swap(&P[++left], &P[end]);

		if (left > mid) findMedian(direction, P, start, left-1);
		else if (left < mid) findMedian(direction, P, left+1, end);
		else return P[mid].y;
	}
	else {  // means x-axis
		if (start == end) return P[mid].x;

		// if (P[right].x < P[end].x) swap(&P[right], &P[end]);
		int pivot = P[end].x;

		while (left < right) {
			while (left < right && P[left].x < pivot)
				left++;
			while (left < right && P[right].x >= pivot)
				right--;
			swap(&P[left], &P[right]);
		}
		if (P[left].x >= pivot) swap(&P[left], &P[end]);
		else swap(&P[++left], &P[end]);

		if (left > mid) findMedian(direction, P, start, left-1);
		else if (left < mid) findMedian(direction, P, left+1, end);
		else return P[mid].x;
	}
}

int x1, y1, x2, y2;
int spots;

template <typename T> long long searchKdTree (KdTree<T>* root, long long sum) {
	if (root == NULL) return 0;

	if (root->lc == NULL && root->rc == NULL) {
		int rx = root->data.x;
		int ry = root->data.y;
		if (x1 <= rx && rx <= x2 && y1 <= ry && ry <= y2) {
			sum += root->data.temp;
			spots++;
		}
		return sum;
	}

	int z1, z2;
	if (!root->splitDirection) {  // x-axis
		z1 = x1; z2 = x2;
	}
	else {  // y-axis
		z1 = y1; z2 = y2;
	}
	if (z1 > root->splitLine)
		return ( sum + searchKdTree(root->rc, 0) );
	else if (z2 < root->splitLine)
		return ( sum + searchKdTree(root->lc, 0) );
	else
		return ( sum + searchKdTree(root->lc, 0) + searchKdTree(root->rc, 0) );
}

int adjustTemp (long long sum) {
	while (sum / 10000 != 0) sum /= 10;
	return sum;
}

int main() {
	GetNumOfStation();
	KdTree<station_type>* root = buildKdTree(stations, 0, n);
	while (GetQuery(&x1, &y1, &x2, &y2)) {
		int temp = 0;
		spots = 0;
		long long sum = searchKdTree(root, 0);
		if (spots) temp = adjustTemp(sum / spots);
		Response(temp);
	}
}