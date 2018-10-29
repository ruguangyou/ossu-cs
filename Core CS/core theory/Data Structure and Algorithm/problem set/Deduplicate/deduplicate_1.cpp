#include <cstdio>
#include <cstring>
using namespace std;

// #define HT_SIZE 601
#define HT_SIZE 809

// FILE* fr;

struct node {
	int frequency;
	char* data;
	node* next;

	node(int f = 0, char* s = NULL) : frequency(f), data(s), next(NULL) {}
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
	setvbuf(stdin,  new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);

	// fr = fopen("input.txt", "r");
	int n, key;

	scanf("%d", &n);
	// fscanf(fr, "%d", &n);

	for (int i = 0; i < n; i++) {
		/* !!! 
		bug: "length of each item is not greater than 40",
		     but there is one extra char '\0', so when allocating
		     memory, the length should be 41 rather than 40 */
		char* s = new char[41];
		scanf("%s", s);
		// fscanf(fr, "%s", s);
		key = hash_str(s);
		//printf("key: %d, string: %s\n", key, s);

		if ( !HT[key] ) {
			HT[key] = new node(1, s);
		}
		else {
			node* temp = HT[key];
			while (temp) {
				if ( !strcmp(s, temp->data) ) { 
					temp->frequency += 1;
					if (temp->frequency == 2) printf("%s\n", s);
					// delete [] s;
					break;
				}
				temp = temp->next;
			}
			if ( !temp ) { 
				temp = HT[key];
				HT[key] = new node(1, s);
				HT[key]->next = temp;
			}
		}
	}

	// fclose(fr);
	return 0;
}