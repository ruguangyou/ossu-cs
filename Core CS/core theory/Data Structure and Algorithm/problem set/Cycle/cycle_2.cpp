/* string match problem, KMP algorithm */
// version 2. 记忆(next表) + 教训() -> 预知(跳过更多不必要的比对)

#include <cstdio>
#include <cstring>

#define MAX 100001

int* buildNext (char* s, int n) {
	int* next = new int[n];
	next[0] = -1;
	for (int i = 1; i < n; i++) {
		// ???
	}
	return next;
}

int main() {
	char* s1 = new char[MAX];
	char* s2 = new char[MAX];

	while (scanf("%s %s", s1, s2) != EOF) {
		int n = strlen(s1);
		if ( n != strlen(s2) ) { printf("NO\n"); continue; }
		if ( n == 1 && s1 == s2) { printf("YES\n"); continue; }
		
		int* next = buildNext(s1, n);
		for (int k = 0; k < n; k++) printf("%d ", next[k]); printf("\n");

		int i = 0, j = 0, c = 0;
		while (j < n) {
			if (s1[j] == s2[i]) {
				j++; i = (i+1) % n;
				if (++c > 2*n) break;
			}
			else {
				if (j == 0) { i++; if (i >= n) break; }
				else j = next[j];
			}
		}
		if (j == n) printf("YES\n");
		else printf("NO\n");

		delete [] next;
	}
}