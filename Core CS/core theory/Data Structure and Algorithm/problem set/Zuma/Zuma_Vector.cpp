#include <cstdio>
#include <cstring>

using namespace std;
#define MAX_INIT_SIZE 10000  // 0 <= length of initial sequence <= 10^4
typedef int Rank;

/* Zuma game!
   Given both the initial sequence and the insertion series,
   you are now asked by the fans to provide a playback tool
   for replaying their games. In other words, the sequence of
   beads after all possible elimination as a resuly of each
   insertion should be calculated. */

int insert(char* s, int m, Rank k, char c) {
	for (int i = m; i > k ; s[i] = s[--i]);
	s[k] = c;
	s[m+1] = '\0';
	return m+1;
}

Rank three(char* s, int m, Rank r) {
/* Return the rank the the first element in a subsequence 
   that has three elements of the same color. */
	if (m < 3) { return -1; }

	if (r > 1 && s[r-2] == s[r]) { return r-2; }
	else if (r > 0 && s[r-1] == s[r]) { r = r-1; }

	if (s[r+2] == s[r]) { return r; }
	else { return -1; }
}

int removeThree(char* s, int m, Rank r) {
	int i = r;
	for (int j = r + 3; j < m; s[i++] = s[j++]);
	s[i] = '\0';
	return i;
}


int main() {
	setvbuf(stdin,  new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	char s[MAX_INIT_SIZE];  // initial sequence
	int n;  // number of operations
	int m;  // length of initial sequence
	Rank k, r;  // rank
	char c; // a capital letter that representing a color

	scanf("%s", s);
	scanf("%d", &n);
	m = strlen(s);

	for (int i = 0; i < n; i++) {
		scanf("%d %c", &k, &c);
		m = insert(s, m, k, c);
		r = k;
		while ( (r = three(s, m, r)) >= 0 ) {
			m = removeThree(s, m, r);
		}
		if (m > 0) { printf("%s\n", s); }
		else { printf("-\n"); }
	}
}