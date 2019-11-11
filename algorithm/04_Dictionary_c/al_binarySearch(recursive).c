#include <stdio.h>
#include <stdlib.h>

int rBinarySearch(int *arr, int l, int r, int k) {
	int mid = (int)(((l + r) / 2.0) + 0.5);
	if (l > r)
		return r;

	if (arr[mid] == k)
		return mid;
	else if (arr[mid] > k)
		rBinarySearch(arr, l, mid - 1, k);
	else // arr[mid] < k
		rBinarySearch(arr, mid + 1, r, k);
}

int main(void) {
	int* arr;
	int n, k, target;

	scanf("%d %d", &n, &k);
	arr = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		scanf("%d", &arr[i]);

	if (k < arr[0])
		target = -1;
	else
		target = rBinarySearch(arr, 0, n - 1, k);
	printf(" %d", target);

	free(arr);
}