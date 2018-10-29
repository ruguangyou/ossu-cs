#include <cstdio>

int main() {
	int* p = new int[10];
	p[0] = 0;
	delete [] p;
}