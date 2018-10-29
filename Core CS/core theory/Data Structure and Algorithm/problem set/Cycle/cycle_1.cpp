/* string match problem, KMP algorithm */
// version 1. 记忆(next表) -> 预知(跳过一些重复的比对)

#include <cstdio>
#include <cstring>

#define MAX 100001

int* buildNext (char* s, int n) {
	int* next = new int[n];
	next[0] = -1;
	for (int i = 1; i < n; i++) {
		int j = i - 1;
		while (j > 0 && s[next[j]] != s[i-1]) j = next[j];
		if (j == 0) next[i] = 0;
		else next[i] = next[j] + 1;
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