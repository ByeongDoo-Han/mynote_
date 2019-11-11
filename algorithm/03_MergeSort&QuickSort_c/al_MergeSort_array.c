// ********합병 정렬 ***********
// 배열로 구성
#include <stdio.h>
#include <stdlib.h>

int n;

void merge(int*li, int l, int m, int r) {
	int* tmp = (int*)malloc(sizeof(int) * (r+1));
	int i, j, k;

	i = l;
	k = l;
	j = m + 1;
	while (i <= m && j <= r) {
		if (li[i] <= li[j])
			tmp[k++] = li[i++];
		else
			tmp[k++] = li[j++];
	}
	while (i <= m) {
		tmp[k++] = li[i++];
	}
	while (j <= r) {
		tmp[k++] = li[j++];
	}
	for (k = l; k <= r; k++)
		li[k] = tmp[k];
	free(tmp);
}
void rMergeSort(int* li, int l, int r) {
	int m;
	if (l < r) {
		m = (l+r) / 2;
		rMergeSort(li, l, m);
		rMergeSort(li, m+1, r);
		merge(li, l, m, r);
	}
	return;
}
void mergeSort(int* li) {
	rMergeSort(li, 0, (n - 1));
	return;
}
void printlist(int *li) {
	for (int i = 0; i < n; i++)
		printf(" %d", li[i]);

	printf("\n");
}
//int* partition(int* li, int k) {}

int main(void) {
	int* list;
	scanf("%d", &n);

	list = (int*)malloc(sizeof(int)*n);
	for (int i = 0; i < n; i++)
		scanf("%d", &list[i]);
	printlist(list);
	mergeSort(list);
	printlist(list);

	free(list);
	return 0;
}