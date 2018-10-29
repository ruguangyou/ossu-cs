#include <cstdio>
#include <cstddef>
#include <cstring>

using namespace std;

#define MAX_SIZE 10001

struct node {
	char color = ' ';
	node* prev = NULL;
	node* next = NULL;
};

node* header = new node;
node* trailer = new node;

node* insert(int r, char c);
void remove (node* p);
node* judge (node* p);

int main() {
	setvbuf(stdin,  new char[1<<20], _IOFBF, 1<<20);
	setvbuf(stdout, new char[1<<20], _IOFBF, 1<<20);

	char s[MAX_SIZE];
	int n;
	int r, k; char c;

	//scanf("%s", s);  不能处理空串的罪魁祸首，害得lz花了那么多时间
	gets(s);
	header->next = trailer;
	trailer->prev = header;

	for (int m = strlen(s); m; m--) {
		node* temp = new node;
		temp->prev = header; temp->next = header->next;
		header->next = temp; temp->next->prev = temp;
		temp->color = s[m-1];
	}
	scanf("%d", &n);

	node* p = header->next;
	if (n == 0) {
		if (p == trailer) printf("-");
		while (p != trailer) { 
			printf("%c", p->color);
			p = p->next;
		}
		printf("\n");
	}

	while (n--) {
		scanf("%d %c", &r, &c);
		p = insert(r, c);
		while ( (p = judge(p)) != NULL ) ;

		p = header->next;
		if (p == trailer) printf("-");
		while (p != trailer) { 
			printf("%c", p->color);
			p = p->next;
		}
		printf("\n");
	}

}

node* insert(int r, char c) {
	node* temp = header->next;
	while (r--) temp = temp->next;
	node* p = new node;
	p->color = c;
	p->prev = temp->prev; p->next = temp;
	temp->prev = p; p->prev->next = p;
	return p;
}

node* judge (node* p) {
	if (p == header) return NULL;
	int i = 1;
	node* q = p;
	char c = p->color;
	while (p->prev->color == c) {
		p = p->prev;
		i++;
	}
	while (q->next->color == c) {
		q = q->next;
		i++;
	}
	if (i > 2) {
		q = p->prev;
		while (i--) {
			p = q->next;
			remove(p);
		}
		return q;
	}
	return NULL;
}

void remove (node* p) {
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
}