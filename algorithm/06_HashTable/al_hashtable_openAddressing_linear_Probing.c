#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _hash {
	int key;
} Hash;

Hash* hashTable;
int hash_size;

bool isEmpty(Hash*);
void insertItem(int);
int findElement(int);
int hashFunc(int);
int getNextBucket(int, int);
void initHashArray(void);

int main(void) {
	char com;
	int key, n, i = 0;

	scanf("%d %d", &hash_size, &n);
	while (getchar() != '\n') {}

	initHashArray();

	while(1){
		scanf("%c", &com);
		switch (com) {
		case 'i':
			if (i > n)
				continue;
			i++;
			scanf("%d", &key);
			insertItem(key);
			break;
		case 's':
			scanf("%d", &key);
			if (findElement(key) == -1)
				printf("-1\n");
			break;
		case 'e':
			return;
		}
		
		while (getchar() != '\n') {}
	}

	free(hashTable);

	return 0;
}

bool isEmpty(Hash* h) {
	if (h->key == 0)
		return true;
	return false;
}
void insertItem(int k) {
	Hash* h;
	int v, hashed_key, i = 0;

	v = hashFunc(k);
	while (i < hash_size) {
		hashed_key = getNextBucket(v, i);
		h = &hashTable[hashed_key];
		if (isEmpty(h)) {
			h->key = k;
			printf("%d\n", hashed_key);
			return;
		}
		printf("C");
		i++;
	}
	return;
}
int findElement(int k) {
	Hash* h;
	int v, hashed_key, i = 0;

	v = hashFunc(k);
	while (i < hash_size) {
		hashed_key = getNextBucket(v, i);
		h = &hashTable[hashed_key];
		if (isEmpty(h))
			return -1;
		else if (k == h->key) {
			printf("%d %d\n", hashed_key, k);
			return hashed_key;
		}
		i++;
	}
	return -1;
}
int hashFunc(int k) {
	return k % hash_size;
}
int getNextBucket(int k, int i) {
	return (k + i) % hash_size;
}
void initHashArray(void) {
	hashTable = (Hash *)calloc(hash_size, sizeof(Hash));
}