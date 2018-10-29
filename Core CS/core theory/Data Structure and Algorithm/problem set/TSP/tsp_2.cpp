#include <cstdio>
// #include <iostream>

using namespace std;

// 邻接矩阵空间复杂度为O(n^2)，1 <= n <= 10^6, 最坏情况所需空间为10^12
// 一个int数值占 4 Byte，因此4*10^12 B 远远超过256MB的空间限制.
// 不妨转换思路使用邻接表，并结合深度优先搜索策略.
int main() {
	int n, m; int u, v;

	scanf("%d%d", &n, &m);

	int** E = new int*[n];
	for (int i = 0; i < n; i++) { 
		E[i] = new int[n];
		for (int j = 0; j < n; j++)
			E[i][j] = 0;
	}

	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++)
	// 		cout << E[i][j] << ' ';
	// 	cout << endl;
	// }

	for (int i = 0; i < n; i++) {
		scanf("%d%d", &u, &v); u -= 1; v -= 1; 
		E[u][v] = 1;
		...
	}

	for (int i = 0; i < n; i++) delete [] E[i];
	delete [] E;
	return 0;
}