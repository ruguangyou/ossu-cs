#include <cstdio>
#include <cstring>
using namespace std;

#define MAXN 4000000

// struct task {
// 	int priority;
// 	char* name; // the task name is shorter than 8

// 	task(int p = 0, char* s = NULL) : priority(p), name(s) {}
// };

class BinaryHeap {
private:
	unsigned int* priority = new unsigned int[MAXN];
	char** name = new char*[MAXN];
	int size = 0;

	bool isLC (int r) { return (r % 2); }

	void percolateUp() {
		int r = size-1; int temp = r;
		unsigned int p = priority[r];
		char* s = name[r];
		while (r) {
			if (isLC(r)) r = (r-1) / 2;
			else r = (r-2) / 2;

			if (p < priority[r] || (p == priority[r] && strcmp(s, name[r]) < 0)) {
				priority[temp] = priority[r];
				name[temp] = name[r];
			}
			else break;
			temp = r;
		}
		priority[temp] = p;
		name[temp] = s;
	}

	int min_p (int lc, int rc) {
		if (priority[lc] < priority[rc] || (priority[lc] == priority[rc] && strcmp(name[lc], name[rc]) < 0))
			return lc;
		else return rc;
	}

	void percolateDown() {
		int r = 0; int temp = r;
		unsigned int p = priority[r];
		char* s = name[r];
		while (r < size) {
			temp = r;
			if (r*2+1 > size-1) break;
			else if (r*2+2 > size-1) r = r*2 + 1;
			else r = min_p(r*2+1, r*2+2);
			if (priority[r] < p || (priority[r] == p && strcmp(name[r], s) < 0)) {
				priority[temp] = priority[r];
				name[temp] = name[r];
			}
			else break;
		}
		priority[temp] = p;
		name[temp] = s;
	}
	
public:
	int getSize() { return size; }

	void insert(unsigned int p, char* s) {
		priority[size] = p;
		name[size++] = s;
		/* 每插入一个新元素即执行上滤，属于蛮力建堆，
		时间复杂度为 O(n.logn)，效率与排序算法一样，
		然而完全二叉堆只是一个偏序序列，相较于排序的
		全序序列，此建堆算法花费相同的时间复杂度显得
		有些得不偿失 */
		percolateUp();
	}
	void printMin() {
		printf("%s\n", name[0]);
	}
	void reschedule() {
		if (priority[0] >= (unsigned int) (1<<31)) delMin();
		else { priority[0] *= 2; percolateDown(); }
	}
	void delMin() {
		// delete [] name[0];
		priority[0] = priority[--size];
		name[0] = name[size];
		percolateDown();
	}
};

int main() {
	BinaryHeap bh;
	int n, m;
	unsigned int p;
	scanf("%d %d", &n, &m);

	if (n == 0 || m == 0) return 0;

	for (int i = 0; i < n; i++) {
		char* s = new char[9];
		scanf("%d", &p);
		scanf("%s", s);
		bh.insert(p, s);
	}

	for (int j = 0; j < m; j++) {
		if ( bh.getSize() == 0 ) break;
		bh.printMin();
		bh.reschedule();
	}
	return 0;
}