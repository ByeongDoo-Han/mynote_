#include <stdio.h>
#include <stdlib.h>

int binarySearch(int* arr, int l, int r, int k) {

	while (1) {
		if (l > r)
			return l;

		int mid = (int)(((l + r) / 2.0) + 0.5);

		if (arr[mid] == k)
			return mid;
		else if (arr[mid] > k) {
			r = mid - 1;
			continue;
		}
		else if (arr[mid] < k) {
			l = mid + 1;
			continue;
		}

	}
}

int main(void) {
	int* arr;
	int n, k, target;

	scanf("%d %d", &n, &k);
	arr = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		scanf("%d", &arr[i]);

	if (k > arr[n-1])
		target = n;
	else
		target = binarySearch(arr, 0, n - 1, k);
	printf(" %d", target);

	free(arr);
}