### 3주차(재귀)
1
```c
#include<stdio.h>
int sum1toN(int n, int sum) {
	if (n == 1) {
		sum += n;
		return sum;
	}
	else {
		sum += n;
		n--;
		sum1toN(n, sum);
	}
}

int main(){
    int n, sum = 0;
	scanf("%d", &n);

	printf("%d", sum1toN(n,sum));
	return 0;
}
```
2
```c
#include<stdio.h>
#include<math.h>

void toLowerMod(int n, int len) {
	int front;
	front = n / pow(10, len);
	if (len == 0) {
		printf("%d \n", front);
		return;
	}
	else {
		printf("%d \n", front);
		toLowerMod(n - (front*pow(10, len)), len - 1);
	}
}
int main(){

    int n,len = 0, tmp;
	scanf("%d", &n);

	tmp = n;
	while (1) {
		if (tmp <= 0)
			break;
		len++;
		tmp /= 10;
	}
	len--;			// 자리수 구하기

	toLowerMod(n, len);
	return 0;
}
```
3
```c
#include<stdio.h>
void toUpperMod(int n) {
	if (n <= 0) {
		return;
	}
	else {
		printf("%d \n", n % 10);
		toUpperMod(n / 10);
	}
}
int main(){

	int n;
	scanf("%d", &n);

	toUpperMod(n);

	return 0;
}
```
4
```c
#include<stdio.h>
int findmax(int * ar,int n, int max) {
	if (n == 0) {
		return max;
	}
	else {
		max = max > *ar ? max : *ar;
		ar--;
		n--;
		findmax(ar,n, max);
	}
}

int main(){

	int n, max;

	scanf("%d", &n);

	int * ar = (int *)malloc(sizeof(int)*n);
	for (int i = 0; i < n; i++)
		scanf("%d", &ar[i]);

	max = *(ar + n - 1);

	printf("%d", findmax(ar + n - 1, n , max));

	free(ar);


	return 0;
}
```
5
```c
#include<stdio.h>
void rHanoi(int n, char a, char b, char c) {
	if (n == 1) {
		printf("%c %c \n", a, c);
		return;
	}
	rHanoi(n - 1, a, c, b);
	printf("%c %c \n", a, c);
	rHanoi(n - 1, b, a, c);
}

int main(){

	/* Type or paste your code in this area */
    int n;
	char a,b,c;
	a = 'A', b = 'B', c = 'C';

	scanf("%d", &n);
	
	rHanoi(n, a,b,c);
	return 0;
}
```
6
```c
#include<stdio.h>
int gcd(int n, int m) {
	if (n == 0 || m == 0) {
		return n;
	}
	else {
		gcd(m, n%m);
	}
}
int main(){

	int n, m;
	scanf("%d %d", &n, &m);

	printf("%d", gcd(n, m));

	return 0;
}
```
7
```c
#include<stdio.h>
int Countchar(char * ar, char target, int cnt) {
	if (*ar == NULL) {
		return cnt;
	}
	else {
		if (target == *ar)
			cnt++;
		Countchar(++ar, target, cnt);
	}
}
int main(){

	int cnt = 0;
	char ar[100], target;
	char * ptr = ar;

	scanf("%s", ar);
	getchar();

	scanf("%c", &target);
	
	printf("%d", Countchar(ptr, target, cnt));
	return 0;
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTAyMTQzODY0M119
-->