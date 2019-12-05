### 10주차(스택2)
1
```c
#include <stdio.h>
#include <stdlib.h>

typedef char element;
typedef struct _stack {
	element data;
	struct _stack *link;
}Stack;


Stack *top;
int n;

void init() {
	top = NULL;
}
void push(element e) {
	Stack *new_node;
	new_node = (Stack *)malloc(sizeof(Stack));
	if (new_node == NULL) {
		return -1;
	}
	new_node->data = e;
	
	if (top == NULL) {
		new_node->link = NULL;
		top = new_node;
	}
	else {
		new_node->link = top;
		top = new_node;
	}
}
char pop() {
	char c;
	Stack *remove;

	c = top->data;
	remove = top;

	top = top->link;
	free(remove);
	return c;
}
int isOperator(char c) {
	switch(c) {
	case '!':
		return 6;
	case '*':
	case '/':
		return 5;
	case '+':
	case '-':
		return 4;
	case '<':
	case '>':
		return 3;
	case '&':
		return 2;
	case '|':
		return 1;
	}
	return 0;
}
int isOperand(char c) {
	if (isOperator(c))
		return 0;
	else
		return 1;
}
int main() {
	char **exp;
	char s;
	int j = 0;
	int double_oper = 0;
	scanf("%d", &n);

	exp = (char **)malloc(sizeof(char *)*n);
	if (exp == NULL) {
		return -1;
	}
	for (int i = 0; i < n; i++) {
		exp[i] = (char *)malloc(sizeof(char) * 100);
		if (exp[i] == NULL)
			return -1;
		scanf("%s", exp[i]);
		getchar();
	}
	
	for (int i = 0; i < n; i++) {
		init();		// top = NULL
		j = 0;
		while (exp[i][j] != '\0') {
			s = exp[i][j++];
			if (s == '(') {
				push(s);
			}
			else if (s == ')') {
				while (top->data != '(') {
					printf("%c", pop());
				}
				pop();
			}
			else if (isOperand(s)) {
				printf("%c", s);
			}
			else {	// s is an operator
				while (top != NULL && (isOperator(s) <=
					isOperator(top->data))) {
					
					if (double_oper == 1) {
						printf("%c", pop());
						printf("%c", pop());
						double_oper = 0;
						break;
					}
					else if (isOperator(s) == 1 || isOperator(s) == 2) {
						double_oper++;
						break;
					}
					printf("%c", pop());		
				}
				push(s);
			}
		}
		while (top != NULL) {
			printf("%c", pop());
		}
		puts("");
	}
	for (int i = 0; i < n; i++) {
		free(exp[i]);
	}
	free(exp);
	free(top);
	return 0;
}

```
2
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct _stack {
	int data;
	struct _stack *next;
}stack;

stack *top;
int n;

void init() {
	top = NULL;
}
void push(int n) {
	stack *new_node;
	new_node = (stack *)malloc(sizeof(stack));
	if (new_node == NULL) {
		printf("Memory allocation failed.\n");
		return;
	}
	new_node->data = n;

	if (top == NULL) {
		top = new_node;
		new_node->next = NULL;
	}
	else {
		new_node->next = top;
		top = new_node;
	}
}
int pop() {
	int num;
	stack *remove;

	remove = top;
	num = top->data;
	top = top->next;
	free(remove);
	return num;
}
char peek() {
	return top->data;
}
int isEmpty() {
	if (top == NULL)
		return 1;
	return 0;
}
int isFull() {
	if (!isEmpty())
		return 1;
	return 0;
}
int isOperator(char c) {
	switch (c) {
	case '+':
	case '-':
	case '*':
	case '/':
		return 1;
	}
	return 0;
}
int doOperator(char op, int x, int y) {
	switch (op) {
	case '+':
		return x + y;
	case '-':
		return y - x;
	case '*':
		return x*y;
	case '/':
		return y/x;
	}
}
int evaluate(char *exp) {
	int i = 0, sum = 0, a,b,tmp;
	char s, temp[2];

	init();
	while (exp[i] != NULL) {
		s = exp[i];
		if (!isOperator(s)) {
			sprintf(temp, "%c", s);
			tmp = atoi(temp);
			push(tmp);
		}
		else {	// s is an operator
			a = pop();
			b = pop();
			push(doOperator(s, a, b));
		}
		i++;
	}
	return pop();
}
int main() {
	char exp[100];
	int *result;
	scanf("%d", &n);

	result = (int *)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		scanf("%s", exp);
		getchar();
		result[i] = evaluate(exp);
	}

	for (int i = 0; i < n; i++) {
		printf("%d\n", result[i]);
	}
	
	free(top);
	free(result);
	
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE0MTk4ODcxMTRdfQ==
-->