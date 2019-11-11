#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 이진탐색하고 중복내용이 많아서 각 함수별 주석을 다 달지는 않고 어떤 내용을 하는지 위주로 달았습니다.

typedef struct _AVL {
	struct _AVL* parent;
	int key;
	int height;
	struct _AVL* lchild;
	struct _AVL* rchild;
} AVL;

AVL* root;

int mymax(int, int);		// compare to two arguments, return bigger one
bool isInternal(AVL*);		// tree's left or right is not NULL -> return true
bool isExternal(AVL*);		// tree's left and right is NULL -> return true
bool isRoot(AVL*);			// is root?
AVL* sibiling(AVL*);		// find current tree's sibiling
AVL* makeNode(int);			// Make node(dynamic allocation), set key, height(0), lchild, rchild -> return new node
void expandExternal(AVL*);	// make current tree's left child and right child using makeNode func
AVL* reduceExternal(AVL*);	// the node replacing the parent node of the removed node z
AVL* treeSearch(AVL*, int); // search node has key if it is existed -> return targeted node
int findElement(int);		// find element and return that
int removeElement(int);		// remove element which using treeSearch func, deallocate found node, searchAndFixAfterRemoval and return that
void insertItem(int);		// insert element which using treeSerach func, allocate found node, searchAndFixAfterInsertion
AVL* inOrderSucc(AVL*);		// find inorderSearch's Succession and return that
void searchAndFixAfterInsertion(AVL*);	// 1. Update heights and Search for imbalance 2. if imbalance node found, Fix that node, and restruct
void searchAndFixAfterRemoval(AVL*);	// 1, Update heights and Search for imbalance 2. if imbalance node found, FIx that node, restruct (recursively)
bool updateHeight(AVL*);				// Update target node's hieght of lchild and rchild 
bool isBalanced(AVL*);					// Check balancing about node's lchild nad rchild
AVL* restructure(AVL*, AVL*, AVL*);		// restruct imbalanced tree in O(1)
void preOrder(AVL*);					// preOrderCircuit
void putNode(AVL*);						// deallocation

int main(void) {
	char com;
	int key, element;

	root = NULL;

	while (1) {
		scanf("%c", &com);
		if (com == 'q')
			break;

		switch (com) {
		case 'i':
			scanf("%d", &key);
			if (root == NULL) {
				root = makeNode(key);
				expandExternal(root);
				root->parent = NULL;
				root->height = 1;
			}
			else
				insertItem(key);
			break;
		case 'd':
			scanf("%d", &key);
			element = removeElement(key);
			if (element == -1)
				printf("X\n");
			else
				printf("%d\n", element);
			break;
		case 's':
			scanf("%d", &key);
			element = findElement(key);
			if (element == -1)
				printf("X\n");
			else
				printf("%d\n", element);
			break;
		case 'p':
			preOrder(root);
			printf("\n");
			break;
		}

		while (getchar() != '\n') {}
	}

	putNode(root);
	return 0;
}

int mymax(int a, int b) {
	if (a >= b)
		return a;
	else
		return b;
}
bool isInternal(AVL* w) {
	if (w->lchild != NULL || w->rchild != NULL)
		return true;
	return false;
}
bool isExternal(AVL* w) {
	if (w->lchild == NULL && w->rchild == NULL)
		return true;
	return false;
}
bool isRoot(AVL* w) {
	if (w->parent == NULL)
		return true;
	else
		return false;
}
AVL* sibiling(AVL* w) {
	if (isRoot(w))
		return NULL;
	if (w->parent->lchild == w)
		return w->parent->rchild;
	else
		return w->parent->lchild;
}
AVL* makeNode(int k) {
	AVL* new_node = (AVL*)malloc(sizeof(AVL));
	new_node->key = k;
	new_node->height = 0;
	new_node->lchild = NULL;
	new_node->rchild= NULL;
	
	return new_node;
}
void expandExternal(AVL* w) {
	w->lchild = makeNode(0);
	w->rchild = makeNode(0);

	w->height = 1;
	w->lchild->height = 0;
	w->lchild->height = 0;

	w->lchild->parent = w;
	w->rchild->parent = w;
}
AVL* treeSearch(AVL* v, int k) {
	if (isExternal(v))
		return v;
	if (k == v->key)
		return v;
	else if (k < v->key)
		return treeSearch(v->lchild, k);
	else
		return treeSearch(v->rchild, k);
}
int findElement(int k) {
	AVL* w;
	w = treeSearch(root, k);

	if (isExternal(w))
		return -1;
	else
		return w->key;
}
void insertItem(int k) {
	AVL* w;
	w = treeSearch(root, k);

	if (isInternal(w))
		return;
	expandExternal(w);
	w->key = k;
	searchAndFixAfterInsertion(w);
}
void searchAndFixAfterInsertion(AVL* w) {
	AVL* z, * y, * x;

	// Update heights and search for imbalance
	w->lchild->height = 0;
	w->rchild->height = 0;
	w->height = 1;

	if (isRoot(w))
		return;

	z = w->parent;
	while (updateHeight(z) && isBalanced(z)) {
		if (isRoot(z))
			return;
		z = z->parent;
	}
	if (isBalanced(z))
		return;
	// Fix imbalance
	if (z->lchild->height > z->rchild->height)
		y = z->lchild;
	else
		y = z->rchild;

	if (y->lchild->height > y->rchild->height)
		x = y->lchild;
	else
		x = y->rchild;

	restructure(x, y, z);
	return;
}
bool updateHeight(AVL* w) {
	AVL* l, * r;
	int h;
	l = w->lchild;
	r = w->rchild;
	h = mymax(l->height, r->height) + 1;

	if (h != w->height) {
		w->height = h;
		return true;
	}
	else
		return false;
}
bool isBalanced(AVL* w) {
	AVL* l, * r;
	l = w->lchild;
	r = w->rchild;
	return (abs(l->height - (r->height)) < 2);
}
AVL* restructure(AVL* x, AVL* y, AVL* z) {
	AVL* a, * b, * c;
	AVL* T0, * T1, * T2, * T3;
	// Assign inorder listings of (x, y, z) and their subtrees to (a, b, c) and (T0, T1, T2, T3), resp.
	/* b = y인 단일회전 */
	if (z->key < y->key && y->key < x->key) {
		a = z; b = y; c = x;
		T0 = a->lchild; T1 = b->lchild; T2 = c->lchild; T3 = c->rchild;
	}
	else if (x->key < y->key && y->key < z->key) {
		a = x; b = y; c = z;
		T0 = a->lchild; T1 = a->rchild; T2 = b->rchild; T3 = c->rchild;
	}
	/* b = x인 이중회전 */
	else if (z->key < x->key && x->key < y->key) {
		a = z; b = x; c = y;
		T0 = a->lchild; T1 = b->lchild; T2 = b->rchild; T3 = c->rchild;
	}
	else {
		a = y; b = x; c = z;
		T0 = a->lchild; T1 = b->lchild; T2 = b->rchild; T3 = c->rchild;
	}

	// Replace the subtree rooted at z with a new subtree rooted at b
	if (isRoot(z)) {
		root = b;
		b->parent = NULL;
	}
	else if (z->parent->lchild == z) {
		z->parent->lchild = b;
		b->parent = z->parent;
	}
	else {// {z->parent->rchild == z)
		z->parent->rchild = b;
		b->parent = z->parent;
	}

	// Let T0 and T1 be the left and the right subtree of a, resp
	a->lchild = T0;
	T0->parent = a;
	a->rchild = T1;
	T1->parent = a;
	updateHeight(a);

	// Let T2 and T3 be the left and the right subtree of c, resp.
	c->lchild = T2;
	T2->parent = c;
	c->rchild = T3;
	T3->parent = c;
	updateHeight(c);

	// Let a and c be the left and the right child of b, resp.
	b->lchild = a;
	a->parent = b;
	b->rchild = c;
	c->parent = b;
	updateHeight(b);

	return b;
}
void preOrder(AVL* v) {
	if (isExternal(v))
		return;
	printf(" %d", v->key);
	preOrder(v->lchild);
	preOrder(v->rchild);
}
void putNode(AVL* v) {
	if (isExternal(v))
		return;
	putNode(v->lchild);
	putNode(v->rchild);
	free(v);
}

AVL* inOrderSucc(AVL* w) {
	w = w->rchild;
	if (isExternal(w))
		return NULL;
	while (isInternal(w->lchild)) {
		w = w->lchild;
	}
	return w;
}
AVL* reduceExternal(AVL* z) {
	AVL* w, * zs, * g;

	w = z->parent;
	zs = sibiling(z);
	if (zs == NULL)
		return NULL;
	if (isRoot(w)) {
		root = zs;
		zs->parent = NULL;
	}
	else {
		g = w->parent;
		zs->parent = g;
		if (w == g->rchild)
			g->rchild = zs;
		else
			g->lchild = zs;
	}

	free(w);
	free(z);

	return zs;
}
int removeElement(int k) {
	AVL* w, * z, *zs, * y;
	int elem;

	w = treeSearch(root, k);
	if (isExternal(w))
		return -1;
	elem = w->key;
	z = w->lchild;

	if (!isExternal(z))
		z = w->rchild;
	// case1
	if (isExternal(z)) {
		if ((zs = reduceExternal(z)) == NULL)
			return -1;
	}

	// case2
	else {
		y = inOrderSucc(w);
		if (y == NULL)
			return -1;
		z = y->lchild;
		w->key = y->key;
		if ((zs = reduceExternal(z)) == NULL)
			return -1;
	}

	searchAndFixAfterRemoval(zs->parent);
	return elem;
}
void searchAndFixAfterRemoval(AVL* w) {
	AVL* z, *y, *x, *b;
	// update heights and search for imbalance
	z = w;
	while (updateHeight(z) && isBalanced(z)) {
		if (isRoot(z))
			return;
		z = z->parent;
	}
	if (isBalanced(z))
		return;

	// Fix imbalance
	if (z->lchild->height > z->rchild->height)
		y = z->lchild;
	else
		y = z->rchild;
	if (y->lchild->height > y->rchild->height)
		x = y->lchild;
	else if (y->lchild->height < y->rchild->height)
		x = y->rchild;
	else {	// (y->lchild->height == y->rchild->height
		if (z->lchild == y)
			x = y->lchild;
		else // z->rchild == y
			x = y->rchild;
	}

	b = restructure(x, y, z);
	if (isRoot(b))
		return;
	searchAndFixAfterRemoval(b->parent);
}