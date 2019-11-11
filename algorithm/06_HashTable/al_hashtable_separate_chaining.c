#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _node {
	int key;
	struct _node* next;
} Node;
typedef struct _hash {
	Node* node;
} Hash;


Hash *hashTable;
int hash_size; // M

bool isEmpty(Hash *);
Node* makeNode(int);
int findElement(int);
void insertItem(int);
int removeElement(int);
void initBucketArray(void);
int hashFunc(int);
void printTable(void);
void deallocateAll(void);

int main(void) {
	char com;
	int key;

	initBucketArray();
	
	getchar();
	while (1) {
		scanf("%c", &com);

		switch (com) {
		case 'i':
			scanf("%d", &key);
			insertItem(key);
			break;
		case 's':
			scanf("%d", &key);
			printf("%d\n", findElement(key));
			break;
		case 'd':
			scanf("%d", &key);
			printf("%d\n", removeElement(key));
			break;
		case 'p':
			printTable();
			break;
		case 'e':
			return 0;
			break;
		}
		while (getchar() != '\n') {}
	}

	deallocateAll();

	return 0;
}

bool isEmpty(Hash* h) {
	if (h->node == NULL)
		return true;
	else
		return false;
}
Node* makeNode(int k) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->key = k;
	new_node->next = NULL;

	return new_node;
}
int findElement(int k) {
	Hash* target;
	Node* p; // phead
	int hashed_key = hashFunc(k);
	int cnt = 0;

	target = &hashTable[hashed_key];
	if (isEmpty(target))
		return 0;
	else {
		p = target->node;
		while (p != NULL) {
			cnt++;
			if (k == p->key)
				return cnt;
			p = p->next;
		}
		return 0;
	}
}
void insertItem(int k) {
	Hash* target;
	Node* new_node;
	int hashed_key = hashFunc(k);
	
	new_node = makeNode(k);
	target = &hashTable[hashed_key];
	if (isEmpty(target)) {
		target->node = new_node;
	}
	else {
		new_node->next = target->node;
		target->node = new_node;
	}
}
int removeElement(int k) {
	Node* p, *removed; // phead
	int hashed_key = hashFunc(k);
	int cnt = 1;

	p = hashTable[hashed_key].node;
	if (isEmpty(&hashTable[hashed_key]))
		return 0;
	if (k == p->key) {
		hashTable[hashed_key].node = p->next;
		return 1;
	}
	else {
		while (p->next != NULL) {
			cnt++;
			if (k == p->next->key) {
				removed = p->next;
				p->next = p->next->next;
				free(removed);
				return cnt;
			}
			p = p->next;
		}
		return 0;
	}
	
}
void initBucketArray(void) {
	scanf("%d", &hash_size);

	hashTable = (Hash*)malloc(sizeof(Hash) * hash_size);
	
	for (int i = 0; i < hash_size; i++) {
		(hashTable + i)->node = NULL;
	}
	return;
}
int hashFunc(int k) {
	return (k % hash_size);
}
void printTable() {
	Node* p;// phead
	for (int i = 0; i < hash_size; i++) {
		if (!isEmpty(&hashTable[i])) {
			p = hashTable[i].node;
			while (p != NULL) {
				printf(" %d", p->key);
				p = p->next;
			}
		}	
	}
	printf("\n");
	return;
}
void deallocateAll() {
	Node* p, *pnext;
	for (int i = 0; i < hash_size; i++) {
		p = hashTable[i].node;
		if (p == NULL)
			continue;
		
		pnext = p->next;
		while (pnext != NULL) {
			free(p);
			p = pnext;
			pnext = pnext->next;
		}
	}

	free(hashTable);
}