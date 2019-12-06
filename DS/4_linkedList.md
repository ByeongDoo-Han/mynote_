### 5~6주차 (연결리스트)
1
```c
#include <stdio.h>

typedef struct Link {
	char element;

	struct Link *next;
	struct Link *prev;
}LinkedNode;

static int list_len = 2;

LinkedNode* create_node(char data, LinkedNode *next, LinkedNode *prev) {
	LinkedNode * new;

	new = (LinkedNode *)malloc(sizeof(LinkedNode));
	if (new == NULL) {
		exit(1);
	}

	new->element = data;
	new->next = next;
	new->prev = prev;

	return new;
}

void init(LinkedNode *head, LinkedNode *trail) {
	head->prev = NULL;
	trail->next = NULL;

	head->next = trail;
	trail->prev = head;
}

LinkedNode* Find_Node(LinkedNode *phead, int pos) {
	LinkedNode *pTemp = phead;
	if (phead == NULL) {
		printf("invalid position \n");						// 잘못된 node 접근
		return;
	}
	else if (pos < 0 || pos >= list_len - 1) {			// 헤더(시작) ~ 트레일러 (전까지)
		printf("invalid position \n");
		return;
	}
	while (pos > 0) {									// pos만큼 이동 후
		pTemp = pTemp->next;
		pos--;
	}

	return pTemp;										// 이동한 node 주소값 반환
}


void add(LinkedNode **phead, int pos, char item) {
	LinkedNode *p, *q;
	q = create_node(NULL, NULL, NULL);
	p = *phead;

	q->element = item;
	if (pos <= 0 || pos >= list_len) {
		printf("invalid position \n");
		return;
	}
	else if (pos == 1) {
		q->next = p->next;
		q->prev = p;
		p->next->prev = q;
		p->next = q;
		list_len++;
		return;
	}
	else {
		while (pos > 1) {
			p = p->next;
			pos--;
		}
		q->next = p->next;
		q->prev = p;
		p->next->prev = q;
		p->next = q;
		list_len++;
		return;
	}
}

LinkedNode delete(LinkedNode **phead, int pos) {
	LinkedNode *p = *phead, *pnext;

	if (pos <= 0 || pos >= list_len-1) {
		printf("invalid position \n");
		return;
	}
	p = Find_Node(p, pos);

	if (p->prev == NULL || p->next == NULL) {
		printf("invalid position \n");
		return;
	}
	pnext = p->next;
	p->prev->next = p->next;
	p->next->prev = p->prev;
	list_len--;
	free(p);

	return *pnext;
}

void get_entry(LinkedNode **phead, int pos) {
	LinkedNode *p = *phead;

	if (pos <= 0 || pos >= list_len-1) {
		printf("invalid position \n");
		return;
	}
	p = Find_Node(p, pos);

	if (p->prev == NULL || p->next == NULL) {
		printf("invalid position \n");
		return;
	}

	printf("%c\n", p->element);
	return;
}

void print(LinkedNode **phead) {
	LinkedNode *p = (*phead)->next;		// 1번째 시작

	if (p == NULL) {
		printf("invalid position \n");
		exit(1);
	}
	while (p->next != NULL) {
		printf("%c", p->element);
		p = p->next;
	}
	printf("\n");
}



int main() {
	LinkedNode *h = NULL, *t = NULL;
	char op, item;
	int pos;

	h = create_node('H', NULL, NULL);
	t = create_node('T', NULL, NULL);

	int cnt;

	init(h, t);		// init phead

	scanf("%d", &cnt);
	while (getchar() != '\n');

	for (int i = 0; i < cnt; i++) {
		scanf("%c", &op);
		switch (op) {
		case 'A':
			scanf("%d %c", &pos, &item);
			add(&h, pos, item);
			break;
		case 'D':
			scanf("%d", &pos);
			delete(&h, pos);
			break;
		case 'G':
			scanf("%d", &pos);
			get_entry(&h, pos);
			//printf("%c", get_entry(&h, pos));
			break;
		case 'P':
			print(&h);
			break;
		default:
			printf("wrong argument function \n");
			break;
		}
		while (getchar() != '\n');
	}
	free(h);
	free(t);
}


```
2
```c
#include <stdio.h>

typedef struct polynomial {
	int coef;
	int exp;

	struct polynomial *next;
}Poly;

Poly *header[2];

void init() {
	for (int i = 0; i < 2; i++) {
		header[i] = (Poly *)malloc(sizeof(Poly));
		header[i]->next = NULL;
	}
}

Poly *appendTerm(int coef, int exp, Poly **phead) {
	Poly *new = (Poly *)malloc(sizeof(Poly));

	new->coef = coef;
	new->exp = exp;
	new->next = NULL;

	(*phead)->next = new;
	*phead = new;
	return *phead;
}

Poly *addPoly(Poly *x, Poly *y) {
	Poly *result = (Poly *)malloc(sizeof(Poly));
	result->next = NULL;

	Poly *p, *q, *r;
	p = x->next;
	q = y->next;
	r = result;

	while (p != NULL && q != NULL) {
		if (p->exp > q->exp) {
			appendTerm(p->coef, p->exp, &r);
			p = p->next;
		}
		else if (p->exp < q->exp) {
			appendTerm(q->coef, q->exp, &r);
			q = q->next;
		}
		else {
			int sum = p->coef + q->coef;
			if (sum != 0) {
				appendTerm(sum, p->exp, &r);
			}
			p = p->next;
			q = q->next;
			
		}
	}

	/*remains x*/
	while (p != NULL) {
		appendTerm(p->coef, p->exp,&r);
		p = p->next;
	}

	/*remains y*/
	while (q != NULL) {
		appendTerm(q->coef, q->exp, &r);
		q = q->next;
	}
	return result;
}

void print_list(Poly *list){
	Poly *p = list->next;

	while (p != NULL) {
		printf(" %d %d", p->coef, p->exp);
		p = p->next;
	}
	return;
}

int main() {
	int n, m, coef, exp;
	Poly *first = NULL, *second = NULL, *poly = NULL;

	init();
	first = header[0];
	second = header[1];

	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &coef, &exp);
		first = appendTerm(coef, exp, &first);
	}

	scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &coef, &exp);
		second = appendTerm(coef, exp, &second);
	}

	first = header[0];
	second = header[1];

	poly = addPoly(first, second);
	print_list(poly);

	free(first);
	free(second);
	free(poly);
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEyNTUzNjMzODNdfQ==
-->