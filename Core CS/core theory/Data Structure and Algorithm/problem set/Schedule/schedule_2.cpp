/* optimize: Floyd heapification algorithm */

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

	void percolateUp(int r) {
		int temp = r;
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

	void percolateDown(int r) {
		int temp = r;
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
	}

	void printMin() {
		printf("%s\n", name[0]);
	}
	
	void reschedule() {
		if (priority[0] >= (unsigned int) (1<<31)) delMin();
		else { priority[0] *= 2; percolateDown(0); }
	}
	
	void delMin() {
		// delete [] name[0];
		priority[0] = priority[--size];
		name[0] = name[size];
		percolateDown(0);
	}

	void heapify_topDown() {
		/* 自上而下的上滤，最坏情况 O(n.logn)
		按照二叉树的拓扑结构，底部节点远远多于顶部节点（指数级），
		所以整体的建堆复杂度可以看作取决于底部节点的上滤，
		而最坏情况是底部节点到树根，即整个树的高度 */
		for (int r = 1; r < size; r++) percolateUp(r);
	}

	void heapify_bottomUp() {
		/* 自下而上的下滤，线性复杂度 O(n)
		同洋由于二叉树的拓扑结构，底部节点远远多于顶部节点，
		整体建堆复杂度取决于底部节点的下滤，
		而最坏情况是底部节点到叶节点，即该底部节点的深度，
		相较于上滤，此方法效率高得多 */
		for (int r = size-1; r >= 0; r--) percolateDown(r);
	}
};

int main() {
	setvbuf(stdin,  new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);

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
		// bh.heapify_topDown();
		
		bh.heapify_bottomUp();

	for (int j = 0; j < m; j++) {
		if ( bh.getSize() == 0 ) break;
		bh.printMin();
		bh.reschedule();
	}
	return 0;
}