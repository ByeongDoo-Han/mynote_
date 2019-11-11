#include <stdio.h>
#include <stdlib.h>


// 중복키 없음
typedef struct _node {
	struct _node* parent;	// 부모를 가리키는 포인터
	int key;				// 키
	struct _node* lchild;	// 왼쪽자식
	struct _node* rchild;	// 오른쪽자식
}Node;

Node *root;

int isInternal(Node* v) {	// 내부노드인지?
	if (v->lchild != NULL || v->rchild != NULL)
		return 1;
	else
		return 0;
}
int isExternal(Node* v) {	// 외부노드인지?
	if (v->lchild == NULL && v->rchild == NULL)
		return 1;
	else
		return 0;
}
Node* sibiling(Node* w) {	// 형제노드 반환
	if (w == root)
		return NULL;
	if (w->parent->lchild == w)
		return w->parent->rchild;
	else
		return w->parent->lchild;
}
Node* makeNode(int key) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->key = key;
	new_node->lchild = NULL;
	new_node->rchild = NULL;

	return new_node;
}
void expandExternal(Node* v) {
	v->lchild = makeNode(0);
	v->rchild = makeNode(0);

	v->lchild->parent = v;
	v->rchild->parent = v;
}
/* 트리 탐색 */
// 1. 외부노드면 return 외부노드
// 2. 적중하면 return 적중한노드
// 3. 노드v의 key가 찾을려는 key보다 작을경우 -> tree 왼쪽 탐색
//								  클경우 -> tree 오른쪽 탐색
Node* treeSearch(Node *v,int k) {
	if (isExternal(v))
		return v;

	if (k == v->key)
		return v;
	else if (k < v->key)
		treeSearch(v->lchild, k);
	else
		treeSearch(v->rchild, k);
}
/* 원소찾기 */
// 1. treeSearch함수로 k를 key로가진 node를 찾고
// 2. 원소반환
int findElement(int k) {
	Node* w;
	w = treeSearch(root, k);

	if (isExternal(w))
		return -1;
	else
		return w->key;
}
/* 원소삽입 */
// 1. key를 가진 node가 있으면 return
// 2. 없으면 외부노드를 확장하여 key를 삽입
void insertItem(int k) {
	Node* w;
	w = treeSearch(root, k);

	if (isInternal(w))
		return;
	else {
		expandExternal(w);
		w->key = k;
	}
}
/* 중위순회후계자 찾기 */
// 1. w node의 오른쪽 자식을 시작으로해서
// 2. 외부노드면 return
// 3. 왼쪽자식이 내부노드일경우에만 왼쪽자식으로 이동
// 4. return 왼쪽자식인 마지막 내부노드
Node* inOrderSucc(Node* w) {
	w = w->rchild;
	if (isExternal(w))
		return NULL;
	while (isInternal(w->lchild)) {
		w = w->lchild;
	}
	return w;
}
/* 노드 줄이기 */
// 1. w는 z의 parent로 설정
// 2. zs를 z의 형제노드로 정하고 만약 NULL이면 return NULL
// 3-1. w가 root면 zs를 root로 이동하고 zs의 부모를 NULL로 치환
// 3-2. w가 root가 아니면 g를 w의 부모로하여 zs의 부모를 g로 링크하고 w가 g의 어떤자식인지를
//		구해서 zs를 g의 어떤자식일지를 정함.
// 4. free(w), free(z)
// 5. return zs node
Node* reduceExternal(Node* z) {
	Node* w, *zs, *g;

	w = z->parent;
	zs = sibiling(z);
	if (zs == NULL) {
		return NULL;
	}
	if (w == root) {
		root = zs;
		zs->parent = NULL;
	}
	else {
		g = w->parent;
		zs->parent = g;
		if (w == g->lchild)
			g->lchild = zs;
		else
			g->rchild = zs;
	}
	free(w);
	free(z);

	return zs;
}
/* 원소 삭제 */
// 1. k를 key로 가진 node를 찾고 만약 외부노드면 NoSuchKey 반환(Key없음)
// 2. key를 elem 변수에 저장하고 w의 왼쪽자식으로 z를 설정
// 3. 만약 z가 내부노드면 z를 w의 오른쪽자식으로 설정
// 4-1. {case 1} z가 외부노드면 reduceExternal(z)를 하고 return 값을 확인
// 4-2. {case 2} z가 내부노드면 inOrderSucc으로 중위순회 후계자를 찾아 y에 저장
//		y의 왼쪽자식을 z로 하여 w에 y를 복사한 후 z에 대해 reduceExternal을 진행하고 return 값을 확인
// 5. return elem
int removeElement(int k) {
	Node* w, *z, *y;
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
		if (reduceExternal(z) == NULL)
			return -1;
	}

	// case2
	else {
		y = inOrderSucc(w);
		if (y == NULL)
			return -1;
		z = y->lchild;
		w->key = y->key;
		if (reduceExternal(z) == NULL)
			return -1;
	}

	return elem;
}
/* 전위순회 */
void preOrder(Node* v) {
	if (isExternal(v))
		return;
	printf(" %d", v->key);
	preOrder(v->lchild);
	preOrder(v->rchild);
}
/* 노드 반환 */
void putNode(Node *v) {
	if (isExternal(v))
		return;
	putNode(v->lchild);
	putNode(v->rchild);
	free(v);
}

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

