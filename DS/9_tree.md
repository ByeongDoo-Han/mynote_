### 12~14 트리
1
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct _tree {
	int data;
	struct _tree *left;
	struct _tree *right;
}tree;

tree F8 = { 80, NULL, NULL };
tree F7 = { 130, NULL, NULL };
tree F6 = { 120, &F7, &F8 };
tree F3 = { 50, NULL, &F6 };

tree F5 = { 90, NULL, NULL };
tree F4 = { 70 , NULL, NULL };
tree F2 = { 30, &F4, &F5 };

tree F1 = { 20, &F2, &F3 };
tree *root = &F1;

int main() {
	tree *t, *tmp = NULL;
	t = (tree *)malloc(sizeof(tree)*8);
	t[0] = F1;
	t[1] = F2;
	t[2] = F3;
	t[3] = F4;
	t[4] = F5;
	t[5] = F6;
	t[6] = F7;
	t[7] = F8;

	int n;
	scanf("%d", &n);

	tmp = &t[n - 1];
	if (n > 8)
		printf("-1");
	else if (tmp->left == NULL && tmp->right == NULL)
		printf("%d", tmp->data);
	else if (tmp->left == NULL)
		printf("%d %d", tmp->data, tmp->right->data);
	else if (tmp->right == NULL)
		printf("%d %d", tmp->data, tmp->left->data);
	else
		printf("%d %d %d", tmp->data, tmp->left->data, tmp->right->data);

	free(t);
}
```
2-1
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct _tree {
	int id;
	int data;
	struct _tree *left, *right;
}tree;

tree F8 = { 8, 80, NULL, NULL };
tree F7 = { 7, 130 , NULL, NULL };
tree F6 = { 6, 120, &F7, &F8 };
tree F5 = { 5, 90, NULL, NULL };
tree F4 = { 4, 70, NULL, NULL };
tree F3 = { 3, 50, NULL, &F6 };
tree F2 = { 2, 30, &F4, &F5 };
tree F1 = { 1, 20, &F2, &F3 };

tree *root = &F1;
tree temp;

int order, node_id;

int isInternal(tree *v) {
	if (v->left == NULL && v->right == NULL)
		return 0;
	return 1;
}

void preOrder(tree *t) {
	if (t == NULL)
		return;
	printf(" %d", t->data);
	if (isInternal(t)) {
		preOrder(t->left);
		preOrder(t->right);
	}
}

void inOrder(tree *t) {
	if (t == NULL)
		return;
	if (isInternal(t))
		inOrder(t->left);
	printf(" %d", t->data);
	if (isInternal(t))
		inOrder(t->right);
}

void postOrder(tree *t) {
	if (t == NULL)
		return;
	if (isInternal(t)) {
		postOrder(t->left);
		postOrder(t->right);
	}
	printf(" %d", t->data);
}

void search(tree *v, int id) {
	if (v == NULL)
		return;
	if (id == v->id) {
		temp = *v;
		return;
	}
	if (isInternal(v)) {
		search(v->left, id);
		search(v->right, id);
	}
}
int main() {
	scanf("%d %d", &order, &node_id);
	if (node_id > 8) {
		printf("-1");
		return 0;
	}
	search(root, node_id);

	switch (order) {
	case 1:
		preOrder(&temp);
		break;
	case 2:
		inOrder(&temp);
		break;
	case 3:
		postOrder(&temp);
		break;
	}
	return 0;


}
```
2-2
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct _tree {
	int id;
	int data;
	struct _tree *left, *right;
}tree;

tree F8 = { 8, 80, NULL, NULL };
tree F7 = { 7, 130 , NULL, NULL };
tree F6 = { 6, 120, &F7, &F8 };
tree F5 = { 5, 90, NULL, NULL };
tree F4 = { 4, 70, NULL, NULL };
tree F3 = { 3, 50, NULL, &F6 };
tree F2 = { 2, 30, &F4, &F5 };
tree F1 = { 1, 20, &F2, &F3 };

tree *root = &F1;
tree temp;

int order, node_id;
int sum = 0;

int isInternal(tree *v) {
	if (v->left == NULL && v->right == NULL)
		return 0;
	return 1;
}

void preOrder_SUM(tree *t) {
	if (t == NULL)
		return;
	sum += t->data;
	if (isInternal(t)) {
		preOrder_SUM(t->left);
		preOrder_SUM(t->right);
	}
}

void search(tree *v, int id) {
	if (v == NULL)
		return;
	if (id == v->id) {
		temp = *v;
		return;
	}
	if (isInternal(v)) {
		search(v->left, id);
		search(v->right, id);
	}
}
int main() {
	scanf("%d", &node_id);
	if (node_id > 8) {
		printf("-1");
		return 0;
	}
	search(root, node_id);

	preOrder_SUM(&temp);

	printf("%d", sum);
}
```
3-1
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct _Tree {
	int id;
	struct _Tree *left, *right;
}Tree;

Tree *root, *temp;

void init() {
	int id, left, right;
	Tree *left_node, *right_node;
	scanf("%d %d %d", &id, &left, &right);
	root->id = id;
	
	left_node = (Tree *)malloc(sizeof(Tree));
	right_node = (Tree *)malloc(sizeof(Tree));

	left_node->id = left;
	right_node->id = right;
	left_node->left = NULL;
	left_node->right = NULL;
	right_node->left = NULL;
	right_node->right = NULL;

	root->left = left_node;
	root->right = right_node;

}

int isInternal(Tree *v) {
	if (v->left == NULL && v->right == NULL)
		return 0;
	return 1;
}

void search(Tree *v, int id) {
	if (v == NULL)
		return;
	if (id == v->id) {
		temp = v;
		return;
	}
	if (isInternal(v)) {
		search(v->left, id);
		search(v->right, id);
	}
}

void make_node(Tree *v) {
	int id, left, right;
	Tree *left_node, *right_node;

	left_node = (Tree *)malloc(sizeof(Tree));
	right_node = (Tree *)malloc(sizeof(Tree));

	scanf("%d %d %d", &id, &left, &right);

	if (left == 0)
		left_node = NULL;
	if (right == 0)
		right_node = NULL;

	search(root, id);

	if (left_node != NULL) {
		left_node->id = left;
		left_node->left = NULL;
		left_node->right = NULL;
	}
	if (right_node != NULL) {
		right_node->id = right;
		right_node->left = NULL;
		right_node->right = NULL;
	}

	temp->left = left_node;
	temp->right = right_node;
	
}

void tree_search(Tree *v, char c) {
	printf(" %d", v->id);
	if (c == 'R')
		temp = v->right;
	else if (c == 'L')
		temp = v->left;
}

void deallocation(Tree *v) {
	if (v == NULL)
		return;
	if (isInternal(v)) {
		deallocation(v->left);
		deallocation(v->right);
	}
	free(v);
}
int main() {
	int n, m, j;
	char *com;

	scanf("%d", &n);
	root = (Tree *)malloc(sizeof(Tree));
	init();

	for (int i = 0; i < n - 1; i++) {
		make_node(root);
	}
	
	scanf("%d", &m);
	getchar();
	com = (char *)malloc(sizeof(char)*n);

	for (int i = 0; i < m; i++) {
		scanf("%s", com);
		getchar();
		j = 0;
		temp = root;
		while (com[j]) {
			tree_search(temp, com[j]);
			j++;
		}
		printf(" %d", temp->id);
		printf("\n");
	}


	free(com);
	deallocation(root);
	root = NULL;
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEzNTkwOTg4OThdfQ==
-->