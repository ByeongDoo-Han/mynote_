### 7주차(집합)
1
```c
#include <stdio.h>
#include <malloc.h>

#define TRUE 1
#define FALSE 0

typedef int element;
typedef struct ListNode
{
	element data;
	struct ListNode *link;
}ListNode;

ListNode *search(ListNode* node, int position) {
	ListNode *p = node;
	if (position < 0) return NULL;
	for (int i = 0; i < position; i++) {
		p = p->link;
	}
	return p;
}
void insert_node(ListNode **phead, int pos, ListNode *new_node) {
	ListNode *p;
	if (*phead == NULL) {
		new_node->link = NULL;
		*phead = new_node;
		return;
	}
	else {
		p = search(*phead, pos - 1);
		new_node->link = p->link;
		p->link = new_node;
	}
}

ListNode *create_node(element data, ListNode *link) {
	ListNode *new_node;

	new_node = (ListNode *)malloc(sizeof(ListNode));
	new_node->data = data;
	new_node->link = link;

	return new_node;
}
int isEmpty(ListNode *p) {
	 if (p == NULL) return 1;
	 return 0;
}
int isMember(int data, ListNode *q) {
	if (q == NULL) return 0;
	while (q != NULL) {
		if (data == q->data) return 1;
		q = q->link;
	}
	return 0;
}
int subset(ListNode *set1, ListNode *set2) {
	if (isEmpty(set1)) return 0;
	else {
		ListNode *p = set1;
		while (p != NULL) {
			if (isMember(p->data, set2) == TRUE) {
				p = p->link;
				if (p == NULL) return 0;
			}
			else {
				return p->data;
			}
		}
	}
}

void free_malloc(ListNode *node) {
	ListNode *p;
	while (node != NULL) {
		p = node;
		node = node->link;
		free(p);
	}
}

int main() {
	ListNode *set1, *set2;
	set1 = NULL;
	set2 = NULL;

	int n, data;
	// set1
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &data);
		insert_node(&set1, i, create_node(data, NULL));
	}

	// set2
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &data);
		insert_node(&set2, i, create_node(data, NULL));
	}
	printf("%d", subset(set1, set2));

	free_malloc(set1);
	free_malloc(set2);
}
```
2
```c
#include <stdio.h>
#include <malloc.h>

typedef int element;
typedef struct ListNode {
	element data;
	struct ListNode *link;
}ListNode;

ListNode *Header1, *Header2;

void Init() {
	Header1 = (ListNode *)malloc(sizeof(ListNode));
	Header2 = (ListNode *)malloc(sizeof(ListNode));

	Header1->link = NULL;
	Header2->link = NULL;
}
ListNode *create_node(element data, ListNode *link) {
	ListNode *new_node;

	new_node = (ListNode *)malloc(sizeof(ListNode));
	new_node->data = data;
	new_node->link = link;

	return new_node;
}

ListNode *search_node(ListNode *node, int position) {
	ListNode *p = node;
	if (position < 0) return p;
	for (int i = 0; i <= position; i++) {
		p = p->link;
	}
	return p;
}
void insert_node(ListNode *phead, int pos, ListNode *new_node) {
	ListNode *p;
	if (phead == NULL) {
		new_node->link = NULL;
		phead = new_node;
	}
	else {
		p = search_node(phead, pos - 1);
		new_node->link = p->link;
		p->link = new_node;
	}
}
int isEmpty(ListNode *node) {
	if (node == NULL) return 1;
	return 0;
}

int isMember(element data, ListNode *node) {
	ListNode *p = node;
	while (p != NULL) {
		if (data == p->data) return 1;
		p = p->link;
	}
	return 0;
}

ListNode *union_node(ListNode *set1, ListNode *set2) {
	int index = 0;
	ListNode *p = set1->link;
	ListNode *q = set2->link;

	ListNode *uni = (ListNode *)malloc(sizeof(ListNode));
	uni->link = NULL;

	if (isEmpty(set1)) return set2;
	while (!isEmpty(p) && !isEmpty(q)) {
		if (p->data < q->data) {
			insert_node(uni, index++, create_node(p->data, NULL));
			p = p->link;
		}
		else if (p->data > q->data){
			insert_node(uni, index++, create_node(q->data, NULL));
			q = q->link;
		}
		else {
			insert_node(uni, index++, create_node(p->data, NULL));
			p = p->link;
			q = q->link;
		}
	}

	while (!isEmpty(p)) {
		insert_node(uni, index++, create_node(p->data, NULL));
		p = p->link;
	}

	while (!isEmpty(q)) {
		insert_node(uni, index++, create_node(q->data, NULL));
		q = q->link;
	}

	return uni;
}

ListNode *intersect(ListNode *set1, ListNode *set2) {
	int index = 0;
	ListNode *p = set1->link;
	ListNode *q = set2->link;

	ListNode *inter = (ListNode *)malloc(sizeof(ListNode));
	//inter->data = NULL;
	inter->link = NULL;

	if (isEmpty(set1)) return set1;
	while (!isEmpty(p) && !isEmpty(q)) {
		if (isMember(p->data, q)) {
			insert_node(inter, index++, create_node(p->data, NULL));
		}
		p = p->link;
	}

	return inter;
}

void display(ListNode *node) {
	if (node->link == NULL) {
		printf(" 0\n");
		return;
	}
	ListNode *p = node->link;
	while (p != NULL) {
		printf(" %d", p->data);
		p = p->link;
	}
	puts("");
	return;
}

void free_malloc(ListNode *node) {
	ListNode *p;
	if (node == NULL) {
		free(node);
		return;
	}
	while (node != NULL) {
		p = node;
		node = node->link;
		free(p);
	}
}


int main() {
	ListNode *unions, *inters;
	int n, data;
	Init();
	

	// set1
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &data);
		insert_node(Header1, i, create_node(data, NULL));
	}

	// set2
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &data);
		insert_node(Header2, i, create_node(data, NULL));
	}

	unions = union_node(Header1, Header2);
	display(unions);
	inters = intersect(Header1, Header2);
	display(inters);

	free_malloc(Header1);
	free_malloc(Header2);
	if (!isEmpty(unions)) free_malloc(unions);
	if (!isEmpty(inters)) free_malloc(inters);
}
``
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTU5NjkyMDY2MF19
-->