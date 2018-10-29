#include <cstdio>
#include "vector.h"

int main() {
	Vector<int> V(5, 5, 0);
	for (int i = 0; i < 5; i++) {
		V[i] = i;
		printf("%d ", V[i]);
	}
}