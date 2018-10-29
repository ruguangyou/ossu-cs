#include <cstdio>
#include <cstring>

#define HT_SIZE 299993

// FILE* fr;

struct node {
	int frequency;
	char* data;

	node(int f = 0, char* s = NULL) : frequency(f), data(s) {}
};

node* HT[HT_SIZE];

int hash_str (char* s) {
	int n = strlen(s);
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += s[i];
	}
	// return (31*sum+2) % HT_SIZE;
	return sum % HT_SIZE;
}

int main() {
	// fr = fopen("input.txt", "r");
	int n, key;

	scanf("%d", &n);
	// fscanf(fr, "%d", &n);

	for (int i = 0; i < n; i++) {
		char* s = new char[41];
		scanf("%s", s);
		// fscanf(fr, "%s", s);

		key = hash_str(s);
		int j = 0;
		// quadratic probe
		while (true) {
			key = (key + j*j) % HT_SIZE;
			if ( !HT[key] ) { HT[key] = new node(1, s); break; }
			else if ( !strcmp(s, HT[key]->data) ) {
				HT[key]->frequency += 1;
				if (HT[key]->frequency == 2) printf("%s\n", s);
				break;
			}
			j++;
		}
	}

	// fclose(fr);
	return 0;
}