// unfinished. Go ahead to version 2.

#include <cstdio>
#include <cstring>

#define MAX 40320  // All possible combinations: 8!

node* status[MAX];
char* queue[MAX]; int head = 0, tail = 0;
int factor[8] = {5040, 720, 120, 24, 6, 2, 1, 1};  // { 7!,6!,5!,4!,3!,2!,1!,0! }

struct node {
	int step;
	int ud, right, rotate;
	node(int s = -1) : step(s), ud(0), right(0), rotate(0) {}
}

void upDown (char* s) {
	char temp;
	temp = s[0]; s[0] = s[7]; s[7] = temp;
	temp = s[1]; s[1] = s[6]; s[6] = temp;
	temp = s[2]; s[2] = s[5]; s[5] = temp;
	temp = s[3]; s[3] = s[4]; s[4] = temp;
	return;
}

void logicRight (char* s) {
	char temp;
	temp = s[3]; s[3] = s[2]; s[2] = s[1]; s[1] = s[0]; s[0] = temp;
	temp = s[4]; s[4] = s[5]; s[5] = s[6]; s[6] = s[7]; s[7] = temp;
	return;
}

void centerClockwise (char* s) {
	char temp;
	temp = s[1]; s[1] = s[6]; s[6] = s[5]; s[5] = s[2]; s[2] = temp;
	return;
}

int findRank (char* s) {
	int r = 0;
	for (int i = 0; i < 8; i++) {
		int temp = 0;
		for (int j = i+1; j < 8; j++)
			if (s[j] < s[i]) temp++;
		r += temp*factor[i];
	}
	return r;
}

void init() {
	char s[8] = "12345678";
	status[0] = new node(0);
	queue[0] = new char[8]; strcpy(queue[0], s); tail++;
	while (head < tail) {
		s = queue[head++];
		queue[tail++] = upDown(s);
	}
	

}

int main() {
	init();

	int n;
	int T[8];
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		int large = 0, small = 0;
		for (int j = 0; j < 8; j++) scanf("%d", T+j);

		printf("%d\n", steps[findRank( )]);
	}

	return 0;
}