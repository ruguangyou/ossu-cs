#include <cstdio>
#include <cstring>

using namespace std;

// 0 <= length of initial sequence <= 10^4
// 0 <= n <= 10^4
// the maximum length of the sequence may be 2*10^4
//  and should take into account the end sign '\0'.
#define MAX_SIZE 20001

int judge (int r);

char s[MAX_SIZE];
char temp[MAX_SIZE];
int len = 0;

int main() {
	setvbuf(stdin,  new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);

	int n;
	int r; char c;
	
	//scanf("%s", s);
	// 经测试，scanf遇到空白符不会读取，直到有非空白符的输入才会读取，
	// 也就是说，如果测试用例提供的是空串，scanf很可能会忽略然后去读接下来
	// 的输入，即 n，从而导致出错.
	//   scanf 不从输入流中取出空白符，仅仅是从输入流中读入字符直至遇到空白符.
	//   gets 从输入流中读入字符，直至遇到'\n'. 此外，gets还会取出'\n'但取出随后就丢弃了.
	//   但可以看出的不同之处是，假如输入流中仅有'\n'字符，scanf会略过，而gets则读取'\n'然后停止然后丢弃'\n'.
	gets(s);
	while (s[len] >= 'A' && s[len] <= 'Z') len++;
	scanf("%d", &n);

	if (n == 0) {
		if (len) printf("%s\n", s);
		else printf("-\n");
	}

	while (n--) {
		scanf("%d %c", &r, &c);
		strcpy(temp, s+r);
		strcpy(s+r+1, temp);
		s[r] = c;
		len++;

		while ( (r = judge(r)) >= 0 );
		if (len) printf("%s\n", s);
		else printf("-\n");
	}
}

int judge (int r) {
	int prev = r;
	int next = r;
	char c = s[r];
	while (prev >= 0  && s[prev] == c) prev--;
	while (next < len && s[next] == c) next++;
	if (next - prev > 3) {
		strcpy(temp, s+next);
		strcpy(s+prev+1, temp);
		len = len - (next-prev-1);
		return prev+1;
	}
	return -1;
}