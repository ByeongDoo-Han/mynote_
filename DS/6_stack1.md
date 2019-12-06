### 9주차 (스택1)
1
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char element;
char *stack;
int size;	// stack size
int cal_n;
int rotate_n;
int top = -1;	// initialize top : -1

void push(char *stack, element e) {
	if (top == size-1) {
		printf("Stack FULL\n");
		return;
	}
	top++;
	stack[top] = e;
}
char pop(char *stack) {
	char ch;
	if (top == -1) {
		return 0;
	}
	ch = stack[top--];
	return ch;
}
void peek(char *stack) {
	if (top == -1) {
		printf("Stack Empty\n");
		return;
	}
	printf("%c", stack[top]);
}
void duplicate(char *stack) {
	char ch;
	if (top == size-1) {
		printf("Stack FULL\n");
		return;
	}
	ch = pop(stack);
	for (int i = 0; i < 2; i++) {
		// 두번 푸쉬햇는데 찬경우 예외 넣어야함
		push(stack, ch);
	}
}
void upRotate(char *stack, int n) {
	char tmp;
	int i = top;
	if (n > top + 1)
		return;

	tmp = stack[top];
	for (int j = 0; j < n - 1; j++) {
		stack[i] = stack[i - 1];
		i--;
	}
	stack[i] = tmp;
}
void downRotate(char *stack, int n) {
	char tmp;
	int i = top - (n-1);
	if (n > top + 1)
		return;

	tmp = stack[top- (n-1)];
	for (int j = 0; j < n - 1; j++) {
		stack[i] = stack[i + 1];
		i++;
	}
	stack[top] = tmp;
}
void print(char *stack) {
	for (int i = top; i >= 0; i--) {
		printf("%c", stack[i]);
	}
	printf("\n");
}
int main() {
	char com[10];	// command
	char ch;

	scanf("%d", &size);
	scanf("%d", &cal_n);
	getchar();

	
	stack = (char *)malloc(sizeof(char)*size);

	for (int i = 0; i < cal_n; i++) {
		scanf("%s", &com);
		getchar();
		if (!strcmp(com,"POP")) {
			ch = pop(stack);
			if (ch == '\0')
				printf("Stack Empty\n");
		}
		else if (!strcmp(com, "PUSH")) {
			push(stack, ch = getchar());
		}
		else if (!strcmp(com, "PEEK")) {
			peek(stack);
		}
		else if (!strcmp(com, "DUP")) {
			duplicate(stack);
		}
		else if (!strcmp(com,"UpR")) {
			scanf("%d", &rotate_n);
			getchar();
			upRotate(stack, rotate_n);
		}
		else if (!strcmp(com,"DownR")) {
			scanf("%d", &rotate_n);
			getchar();
			downRotate(stack, rotate_n);
		}
		else if (!strcmp(com, "PRINT")){
		print(stack);
		}
	}
	
	free(stack);
}
```
2
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char element;
char *stack;
int top, str_len;

void init() {
	top = -1;
}
void push(element e) {
	if (top == str_len) {
		return;
	}
	top += 1;
	stack[top] = e;
}
element pop() {
	char ch;
	if (top == -1) {
		return -1;
	}
	ch = stack[top];
	top--;
	return ch;
}
int isOpen(char c) {
	switch (c) {
	case '(':
		return 1;
	case '[':
		return 2;
	case '{':
		return 3;
	}
	return 0;
}
int isClose(char c) {
	switch (c) {
	case ')':
		return 1;
	case ']':
		return 2;
	case '}':
		return 3;
	}
	return 0;
}
int main() {
	char str[1001];
	int oper_cnt = 0;
	int i = 0, close, remain = 0;
	
	scanf("%[^\n]", str);
	str_len = strlen(str);

	stack = (char *)malloc(sizeof(char)*(str_len+1));
	init();
	while (str[i]) {
		if (isOpen(str[i])) {
			push(str[i]);
			oper_cnt++;
		}
		else if (close = isClose(str[i])) {
			remain = 1;
			if (isOpen(stack[top]) == close) {
				pop();
				remain = 0;
			}
			oper_cnt++;
		}
		i++;
	}

	if (top > -1 || remain == 1)
		printf("Wrong_%d", oper_cnt);
	else
		printf("OK_%d", oper_cnt);
	free(stack);
	return 0;
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE5NTU1NTU0NThdfQ==
-->