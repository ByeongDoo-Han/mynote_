### 2주차(분석)
1
```c
#include <stdio.h>

int modulo(int a, int b) {
	if (a < b)
		return a;
	else 
		return modulo(a-b, b);
}
int main() {
	int a, b;

	scanf("%d %d", &a, &b);

	printf("%d", modulo(a, b));
}
```
2
```c
#include <stdio.h>

int mostOnes(int **arr, int n) {
	int i = 0, j = 0, row = 0;

	while ((i < n)&&(j < n)) {
		if (arr[i][j] == 0) {
			i++;
		}
		else {
			row = i;
			j++;
		}
	}
	return row;
}
int main() {
	int n;
	int ** arr;

	scanf("%d", &n);
	arr = (int **)malloc(sizeof(int*) * n);
	
	for (int i = 0; i < n; i++) {
		arr[i] = (int *)malloc(sizeof(int)*n);
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			scanf("%d", &arr[i][j]);
	}

	printf("%d", mostOnes(arr, n));

	for (int i = 0; i < n; i++)
		free(arr[i]);
	free(arr);
}
```
3
```c
#include <stdio.h>

int * prefixAverages1(int *x, int n) {
	int sum = 0;
	double avg = 0.0;
	int *ptr = x;

	int * arr1 = (int *)malloc(sizeof(int)*n);

	for (int i = 0; i < n; i++) {
		sum = 0;
		ptr = x;
		for (int j = 0; j <= i; j++) {
			sum += *ptr;
			ptr++;
		}
		avg = (double)sum / (i + 1);
		if (avg - (int)avg >= 0.5) {
			arr1[i] = (int)avg + 1;
		}
		else {
			arr1[i] = (int)avg;
		}
	}

	return arr1;
}

int * prefixAverages2(int *x, int n) {
	int sum = 0;
	double avg = 0.0;
	int *ptr = x;

	int * arr2 = (int *)malloc(sizeof(int)*n);

	for (int i = 0; i < n; i++) {
		sum += *ptr;
		ptr++;

		avg = (double)sum / (i + 1);
		if (avg - (int)avg >= 0.5) {
			arr2[i] = (int)avg + 1;
		}
		else {
			arr2[i] = (int)avg;
		}
	}
	return arr2;
}
int main() {
	int n;
	int *arr1, *arr2;
	scanf("%d", &n);

	int * x = (int *)malloc(sizeof(int)*n);		// array X

	for (int i = 0; i < n; i++)
		scanf("%d", &x[i]);

	// A array of prefixAverages1
	arr1 = prefixAverages1(x, n);
	for (int i = 0; i < n; i++)
		printf("%d ", arr1[i]);

	puts("");

	// A array of prefixAverages2
	arr2 = prefixAverages2(x, n);
	for (int i = 0; i < n; i++)
		printf("%d ", arr2[i]);

	//free memory
	free(x);
	free(arr1);
	free(arr2);

}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE3NzYwMzY4M119
-->