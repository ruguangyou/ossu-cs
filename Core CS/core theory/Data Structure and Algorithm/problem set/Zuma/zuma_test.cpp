#include <cstdio>
#include <cstddef>
#include <cstring>

using namespace std;

#include "zuma_list.h"

#define MAX_SIZE 20001

int main() {
	setvbuf(stdin,  new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	char s[MAX_SIZE];
	Zuma<char> z;
	ZumaNode<char>* p;
	char e; int n; Rank r;

	scanf("%s", s);
	for (int i = strlen(s); i > 0; z.insertAsFirst(s[--i]));

	scanf("%d", &n);

	while (n--) {
		scanf("%d %c", &r, &e);
		p = z.insertPred(z[r], e);
		while ( (p = z.sameColor(p)) != NULL ) {
			p = z.removeSameColor(p);
		}
		if (z.size() > 0) {
			for (p=z.first(); p->pred != z.last(); p = p->succ) printf("%c", p->color);
			printf("\n");
		}
		else {
			printf("-\n");
		}
	}
	return 0;
}