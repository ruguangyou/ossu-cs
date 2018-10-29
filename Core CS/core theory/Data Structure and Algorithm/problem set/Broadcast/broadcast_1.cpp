#include <cstdio>

// bug: 由于给点初始化时都赋值为 1，忽略了有些点应赋为 -1的可能性
//      比如，1-2 1-3 5-4 4-3 输出结果是不可行，但实际上这是可行的，
//           只不过第一次遇到 5 的时候应该令该点为 -1.

int main() {
	int n, m; int u, v;
	scanf("%d%d", &n, &m);

	int* V = new int[n];
	for (int i = 0; i < n; i++) V[i] = 0;

	for (int j = 0; j < m; j++) {
		scanf("%d%d", &u, &v); u -= 1; v -= 1;
		if (!V[u]) {
			if (V[v]) { int temp = u; u = v; v = temp; }
			else V[u] = 1;
		}
		if (u == v) continue;
		if (V[v] && V[v] == V[u]) { printf("-1\n"); delete [] V; return 0; }
		V[v] = -V[u];
	}
	printf("1\n");

	delete [] V;
	return 0;
}