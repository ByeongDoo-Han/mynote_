#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
	int elem;
	struct _Node* next;
} Node;

Node* arr[2];

Node* addLast(int elem) {
	Node* new_node = (Node*)malloc(sizeof(Node));

	new_node->elem = elem;
	new_node->next = NULL;

	return new_node;
}
Node* merge(Node* l, Node* r) {
	Node* result = NULL;

	if (l == NULL)
		return r;
	else if (r == NULL)
		return l;

	if (l->elem <= r->elem) {
		result = l;
		result->next = merge(l->next, r);
	}
	else {
		result = r;
		result->next = merge(l, r->next);
	}

	return result;
}

Node** partition(Node* list, int k) {
	Node** parr = arr;
	Node* p;

	p = list;
	parr[0] = list;

	while (k > 1) {
		p = p->next;
		k--;
	}

	parr[1] = p->next;

	p->next = NULL;

	return parr;
}

void mergeSort(Node** list, int k) {
	Node* l = NULL, * r = NULL;
	Node** parr;

	if (k > 1 && *list != NULL) {
		parr = partition(*list, k / 2);
		l = parr[0];
		r = parr[1];

		mergeSort(&l, k / 2);
		mergeSort(&r, (int)((k / 2.0) + 0.5));

		*list = merge(l, r);
	}
}
void printNode(Node* node) {
	Node* p = node->next;
	while (p != NULL) {
		printf(" %d", p->elem);
		p = p->next;
	}
	printf("\n");
}
void putNode(Node** node) {
	Node* removed;
	while (*node != NULL) {
		removed = *node;
		*node = (*node)->next;
		free(removed);
		removed = NULL;
	}
}

int main(void) {
	int n, data;
	Node* head, * p;
	scanf("%d", &n);

	head = (Node*)malloc(sizeof(Node));
	head->next = NULL;

	p = head;

	for (int i = 0; i < n; i++) {
		scanf("%d", &data);

		p->next = addLast(data);
		p = p->next;

	}
	mergeSort(&head->next, n);
	printNode(head);


	putNode(&head);
	return 0;
}

