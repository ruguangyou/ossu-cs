#include <cstdio>

// int: 4 bytes (32 bits);  long: 4 bytes (32 bits)
// long long: 8 bytes (64 bits)

// 输入 0 <= n <= 4*10^6
// 灯塔坐标 0 <= x,y <= 10^8
// 输出彼此照亮的灯塔对，理论上所以配对可能应为 n^2 量级
// int 整数范围为[2^-31, 2^31-1], 大致在10^9量级
// long long 范围为[2^-63, 2^63-1]， 大致10^18量级

#define MAX_INPUT 4000000

int main() {
	int* xs = new int[MAX_INPUT];
	int* ys = new int[MAX_INPUT];
	int n, x, y;
	long long pairs = 0;

	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d%d", &x, &y);
		xs[i] = x; ys[i] = y;
		for (int j = 0; j < i; j++) {
			if ( (x > xs[j] && y > ys[j]) || (x < xs[j] && y < ys[j]) ) pairs++;
		}
	}
	printf("%ld\n", pairs);
	return 0;
}