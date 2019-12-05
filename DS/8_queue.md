### 11주차(큐)
1
```c
#include <stdio.h>
#include <stdlib.h>

int *queue, q;
int front = 0, rear = 0;
void init() {
	for (int i = 0; i < q; i++)
		queue[i] = 0;
}
int isFull() {
	if ((rear + 1) % q == front)
		return 1;
	return 0;
}
int isEmpty() {
	if (rear == front)
		return 1;
	return 0;
}
void print() {
	for (int i = 0; i < q; i++) {
		printf(" %d", queue[i]);
	}
	puts("");
}
void insert(int n) {
	if (isFull()) {
		printf("overflow ");
		print();
		exit(-1);
	}
	rear = (++rear) % q;
	queue[rear] = n;
}
void deletee() {
	if (isEmpty()) {
		printf("underflow\n");
		exit(-1);
	}
	front = (++front) % q;
	queue[front] = 0;
}
int main() {
	int n, num;
	char com;

	scanf("%d", &q);
	queue = (int *)malloc(sizeof(int)*q);
	init();

	scanf("%d", &n);
	getchar();
	for (int i = 0; i < n; i++) {
		scanf("%c", &com);
		switch (com) {
		case 'I':	// insert element in queue
			scanf(" %d", &num);
			insert(num);
			getchar();
			break;
		case 'D':	// delete, change element '0'
			deletee();
			getchar();
			break;
		case 'P':	// display all elements
			print();
			getchar();
			break;
		}
		
	}

	free(queue);
	return 0;
}
```
2
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _deque {
	int data;
	struct _deque *left;
	struct _deque *right;
}deque;

deque *front, *rear;
void init() {
	front = NULL;
	rear = NULL;
}
void add_front(int num) {
	deque *new_node = (deque *)malloc(sizeof(deque));
	new_node->data = num;

	if (front == NULL) {
		new_node->right = NULL;
		new_node->left = NULL;
		front = new_node;
	}
	else {
		new_node->right = front;
		new_node->left = NULL;
		front->left = new_node;
		front = new_node;
	}

	/*find rear*/
	deque *temp = front;
	while (temp->right != NULL)
		temp = temp->right;
	rear = temp;
}
void add_rear(int num) {
	deque *new_node = (deque *)malloc(sizeof(deque));
	new_node->data = num;

	if (rear == NULL) {
		new_node->right = NULL;
		new_node->left = NULL;
		rear = new_node;
	}
	else {
		new_node->right = NULL;
		new_node->left = rear;
		rear->right = new_node;
		rear = new_node;
	}

	/*find front*/
	deque *temp = rear;
	while (temp->left != NULL)
		temp = temp->left;
	front = temp;
}
int delete_front() {
	deque *remove;
	int temp;

	remove = front;
	temp = remove->data;

	front = front->right;
	if (front == NULL) {
		free(remove);
		return -1;
	}
	front->left = NULL;

	free(remove);
	return temp;
}
int delete_rear() {
	deque *remove;
	int temp;

	remove = rear;
	temp = remove->data;

	rear = rear->left;
	if (rear == NULL) {
		free(remove);
		return -1;
	}
	rear->right = NULL;

	free(remove);
	return temp;
}
void print() {
	deque *temp = front;
	while (temp != NULL) {
		printf(" %d", temp->data);
		temp = temp->right;
	}
	printf("\n");
}
int main() {
	int n, num, tmp = 0;
	char com[3];

	scanf("%d", &n);
	getchar();
	init();
	for (int i = 0; i < n; i++) {
		scanf("%s", com);

		if (!strcmp(com, "AF")) {
			scanf("%d", &num);
			add_front(num);
		}
		else if (!strcmp(com, "AR")) {
			scanf("%d", &num);
			add_rear(num);
		}
		else if (!strcmp(com, "DF")) {
			if (tmp == -1) {
				printf("underflow");
				return -1;
			}
			tmp = delete_front();
		}
		else if (!strcmp(com, "DR")) {
			if (tmp == -1) {
				printf("underflow");
				return -1;
			}
			tmp = delete_rear();
		}
		else if (!strcmp(com, "P"))
			print();

		getchar();

		if (front == NULL || rear == NULL)
			init();
	}
	return 0;
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTMwOTE4NjY3MV19
-->