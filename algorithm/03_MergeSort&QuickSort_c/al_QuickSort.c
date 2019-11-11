#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int mid_bucket[2];	// left, right side

void swapElement(int* a, int* b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
int findPivot(int l, int r) {
	int pivot;

	pivot = rand() % (r - l) + l;

	return pivot;
}
int* inplacePartition(int* L, int l, int r, int k) {
	int p;	// pivot
	int i, j, cnt = 0;
	int* result = mid_bucket;

	swapElement(&L[k], &L[r]);	// hide pivot
	p = L[r];

	i = l;
	j = r - 1;

	while (i <= j) {
		while (i <= j && L[i] <= p)
			i++;
		while (i <= j && L[j] >= p)
			j--;
		if (i < j) {
			swapElement(&L[i], &L[j]);
		}
	}
	swapElement(&L[i], &L[r]);

	j = i + 1;
	while (j <= r) {
		if (L[j] == p) {
			swapElement(&L[++i], &L[j]);
			cnt++;
		}
		j++;
	}

	result[0] = i - cnt;
	result[1] = i;

	return result;
}
void inplaceQuickSort(int* L, int l, int r) {
	int k, * p;

	if (l >= r)
		return;
	k = findPivot(l, r);
	p = inplacePartition(L, l, r, k);
	inplaceQuickSort(L, l, p[0] - 1);
	inplaceQuickSort(L, p[1] + 1, r);
}
void printAll(int* L, int size) {
	for (int i = 0; i < size; i++)
		printf(" %d", L[i]);
}

int main() {
	int n;
	int* arr;

	scanf("%d", &n);
	arr = (int*)malloc(sizeof(int) * n);
	srand((unsigned int)time(NULL));
	for (int i = 0; i < n; i++) {
		scanf("%d", &arr[i]);
	}
	inplaceQuickSort(arr, 0, n - 1);
	printAll(arr, n);
	free(arr);
}