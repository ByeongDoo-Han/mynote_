### 1주차(C언어복습)
1
```c
#include <stdio.h>

int sum(int a);

int main() {
	int n, res = 0;

	scanf("%d", &n);

	for (int i = 1; i <= n; i++) {
		res += sum(i);
	}

	printf("%d", res);
}

int sum(int a) {
	
	int ret = 0;

	for (int i = 1; i <= a; i++) {
		ret += i;
	}

	return ret;
}
```

2
```c
#include <stdio.h>

void ABC(int arr[], int k);

int main() {
	int arr[10];
	int k = 0;

	for (int i = 0; i < 10; i++) {
		scanf("%d", &arr[i]);
	}

	for (int i = 0; i < 10; i++) {
		ABC(arr, k);
		k++;
	}

	for (int i = 0; i < 10; i++)
		printf(" %d", arr[i]);
}

void ABC(int arr[], int k) {
	int tmp, max = arr[k];
	int idx = 0;

	for (int j = k; j < 10; j++) {
		if (max < arr[j]) {
			max = arr[j];
			idx = j;
		}

	}

	if (arr[k] < max) {
		tmp = arr[k];
		arr[k] = max;
		arr[idx] = tmp;
	}
	else return 0;

}
```
3
```c
#include <stdio.h>

void swap(int *pta, int *ptb);

int main() {
	
	int n, a, b;
	int arr[50];
	int *p, *q;

	scanf("%d", &n);

	for (int i = 0; i < n; i++)
		scanf("%d", &arr[i]);

	scanf("%d %d", &a, &b);


	p = &arr[a];
	q = &arr[b];

	
	swap(p, q);

	for (int i = 0; i < n; i++)
		printf(" %d", arr[i]);
}

void swap(int *pta, int *ptb) {
	int tmp;

	tmp = *pta;
	*pta = *ptb;
	*ptb = tmp;
}


```
4
```c
#include<stdio.h>

int main(){

	/* Type or paste your code in this area */
    char str[100];
	int len = 0;
	char *p = str, *q;

	scanf("%s", str);

	while (1) {
		if (str[len] == NULL) break;
		else len++;

	}
	
	for (int i = 0; i < len; i++) {
		printf("%s\n", str);

		*(p + len) = *p;
		
		for (int j = 0; j < len; j++) {
			*p = *(p + 1);
			p++;
		}
		
		
		do {
			*p = NULL;
			p++;
		} while (p < str + 100);

		p = str;
		
	}
	
	return 0;
}
```
5
```c
#include <stdio.h>

typedef struct _time {
	int hour;
	int min;
	int sec;
}time;

int main() {
	time t1;
	time t2;

	int h, m, s;

	scanf("%d %d %d", &t1.hour, &t1.min, &t1.sec);
	scanf("%d %d %d", &t2.hour, &t2.min, &t2.sec);

	h = t2.hour - t1.hour;

	if (t2.min < t1.min) {
		m = (60 - t1.min) + t2.min;
		h--;
	}
	else {
		m = t2.min - t1.min;
	}

	if (t2.sec < t1.sec) {
		s = (60 - t1.sec) + t2.sec;
		m--;

	} else {
		s = t2.sec - t1.sec;
	}
	
	printf("%d %d %d", h, m, s);
}


```
6
```c
#include <stdio.h>

typedef struct _student{
	 char name[10];
	 int score;
}student;

int main() {
	student s[5];

	int sum = 0;
	double avg = 0.0;

	for (int i = 0; i < 5; i++) {
		scanf("%s %d", &s[i].name, &s[i].score);
	}

	for (int i = 0; i < 5; i++)
		sum += s[i].score;

	avg = (double)sum / 5;

	for (int i = 0; i < 5; i++) {
		if (s[i].score <= avg)
			printf("%s\n", s[i].name);
	}

}


```
7
```c
#include<stdio.h>

int main(){

	int N, sum = 0;
	int *p = NULL, *q = NULL;

	scanf("%d", &N);

	// arr1 dynamic allocation
	int * arr1 = (int *)malloc(sizeof(int)*N);
	p = arr1;

	for (p = arr1; p < arr1 + N; p++)
		scanf("%d", p);

	// arr2 dynamic allocation
	int * arr2 = (int *)malloc(sizeof(int)*N);
	q = arr2;

	for (q = arr2; q < arr2 + N; q++)
		scanf("%d", q);
    
    // print reverse sum
	for (int i = 0; i < N; i++) {
		sum = *(arr1 + i) + *(arr2 + N - 1 - i);
		printf("%d ", sum);
	}
	
	free(arr1);
	free(arr2);

	return 0;
}
```
8
```c
#include<stdio.h>

typedef struct stu {
	char name[8];
	int kor;
	int eng;
	int math;
	double avg;
} student;

int main(){

    int N;

	scanf("%d", &N);

	// dynamic allocation struct student
	student * s = (student *)malloc(sizeof(student)*N);

	// input student variable's values
	for (int i = 0; i < N; i++) {
		scanf("%s", s[i].name);
		scanf("%d", &s[i].kor);
		scanf("%d", &s[i].eng);
		scanf("%d", &s[i].math);

		s[i].avg = (double)(s[i].kor + s[i].eng + s[i].math) / 3;
	}
	
	// print
	for (int i = 0; i < N; i++) {
		printf("%s %.1f ", s[i].name, s[i].avg);
		if (s[i].kor >= 90 || s[i].eng >= 90 || s[i].math >= 90)
			printf("GREAT ");
		if (s[i].kor < 70 || s[i].eng < 70 || s[i].math < 70)
			printf("BAD");
		puts("");
	}

	free(s);
	return 0;
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMjA3OTUxNjM1NV19
-->