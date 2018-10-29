#include <cstdio>
#include "graphmatrix.h"

#define EDGE 0
#define WEIGHT 0

int main() {
	int n, m;
	int u, v;
	GraphMatrix<int, int> G;

	scanf("%d%d", &n, &m);

	for (int i = 1; i <= n; i++) G.insert(i);

	while (m--) {
		scanf("%d%d", &u, &v);
		G.insert(EDGE, WEIGHT, u, v);
	}

	G.dfs(1);
	printf("dfs");

	return 0;
}