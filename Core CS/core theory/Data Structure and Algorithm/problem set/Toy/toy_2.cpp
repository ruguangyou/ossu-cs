// Why not consider to take inverse operations?
// i.e. { downUp, logicLeft, centerAnticlockwise } rather than
//      { upDown, logicRight, centerClockwise}
// such that it's easier to count the step needed to return to initial state.

#include <cstdio>

#define MAX 40320  // All possible combinations: 8!

struct node {
	int step;  // record the number of steps needed to return to the initial state "12345678"
	int ud, right, rotate;

	node() : step(0), ud(0), right(0), rotate(0) {}
};

node* status[MAX];
int queue[MAX];
int factor[8] = {5040, 720, 120, 24, 6, 2, 1, 1};

int vec2int (int* T) {
	int s = 0;
	for (int i = 0; i < 8; i++) s = s*10 + T[i];
	return s;
}

int findRank (int* T) {
	int r = 0;
	for (int i = 0; i < 8; i++) {
		int temp = 0;
		for (int j = i + 1; j < 8; j++)
			if (T[j] < T[i]) temp++;
		r += temp*factor[i];
	}
	return r;
}

void upDown (int* T) {
	char temp;
	temp = T[0]; T[0] = T[7]; T[7] = temp;
	temp = T[1]; T[1] = T[6]; T[6] = temp;
	temp = T[2]; T[2] = T[5]; T[5] = temp;
	temp = T[3]; T[3] = T[4]; T[4] = temp;
	return;
}

void logicRight (int* T) {
	char temp;
	temp = T[3]; T[3] = T[2]; T[2] = T[1]; T[1] = T[0]; T[0] = temp;
	temp = T[4]; T[4] = T[5]; T[5] = T[6]; T[6] = T[7]; T[7] = temp;
	return;
}

void centerClockwise (int* T) {
	char temp;
	temp = T[1]; T[1] = T[6]; T[6] = T[5]; T[5] = T[2]; T[2] = temp;
	return;
}

void init() {
	int T[8];
	int r;
	int step, ud, right, rotate;
	int head = 0, tail = 0;
	status[0] = new node();
	queue[0] = 12345678; tail++;
	while (head < tail) {
		int s = queue[head++];
		int temp = s;
		for (int i = 7; i >= 0; i--) { T[i] = s % 10; s = (int) s / 10; }
		step = status[findRank(T)]->step;
		ud = status[findRank(T)]->ud;
		right = status[findRank(T)]->right;
		rotate = status[findRank(T)]->rotate;

		upDown(T);
		r = findRank(T);
		if ( !status[r] ) {
			queue[tail++] = vec2int(T);
			status[r] = new node();
			status[r]->ud = (ud + 1) % 2;
			if (status[r]->ud) status[r]->step = step + 1;
			else status[r]->step = step - 1;
		}
		else {
			int temp;
			if ((ud+1) % 2) temp = step + 1;
			else temp = step - 1;
			
			if (temp && temp < status[r]->step) {
				status[r]->step = temp;
				status[r]->ud = (ud + 1) % 2;
			}
		}
		
		s = temp;
		for (int i = 7; i >= 0; i--) { T[i] = s % 10; s = (int) s / 10; }
		logicRight(T);
		r = findRank(T);
		if ( !status[r] ) {
			queue[tail++] = vec2int(T);
			status[r] = new node();
			status[r]->right = (right + 1) % 4;
			if (status[r]->right != 1) status[r]->step = step - 1;
			else status[r]->step = step + 3;
		}
		else {
			int temp;
			if ((right+1)%4 != 1) temp = step - 1;
			else temp = step + 3;
			
			if (temp && temp < status[r]->step) {
				status[r]->step = temp;
				status[r]->right = (right + 1) % 4;
			}
		}

		s = temp;
		for (int i = 7; i >= 0; i--) { T[i] = s % 10; s = (int) s / 10; }
		centerClockwise(T);
		r = findRank(T);
		if ( !status[r] ) {
			queue[tail++] = vec2int(T);
			status[r] = new node();
			status[r]->rotate = (rotate + 1) % 4;
			if (status[r]->rotate != 1) status[r]->step = step - 1;
			else status[r]->step = step + 3;
		}
		else {
			int temp;
			if ((rotate+1)%4 != 1) temp = step - 1;
			else temp = step + 3;
			
			if (temp && temp < status[r]->step) {
				status[r]->step = temp;
				status[r]->rotate = (rotate + 1) % 4;
			}
		}
	}
}

int main() {
	init();

	int n, r;
	int T[8];
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 8; j++) scanf("%d", T+j);
		r = findRank(T);
		if ( status[r] ) printf("%d\n", status[r]->step);
		else printf("-1\n");
	}
	return 0;
}