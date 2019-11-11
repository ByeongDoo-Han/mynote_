#include <stdio.h>
#include <stdlib.h>

/*	HeapSort - only(sole) key OK, overlapped key OK
	
	다음 조건에 맞추어 힙 정렬 프로그램을 작성하라.
1) 순차힙으로 구현한다. 즉, 배열을 이용한 순차트리 형식으로 힙을 저장한다.
2) 연산 효율을 위해 배열 인덱스 0 위치는 사용하지 않고 비워둔다.
3) 데이터구조를 단순화하기 위해 힙의 항목으로써 (키, 원소) 쌍에서 원소를 생략하고 키만 저장하는 것으로 한다.
4) 키들은 중복이 없는 1 이상의 정수로 전제한다 – 즉, 중복 키에 대한 처리는 불필요하다.
5) 최대 키 개수 < 100 으로 전제한다.
6) 1기(힙 생성 단계)에서 삽입식 또는 상향식 가운데 어떤 방식을 사용해도 좋다.
7) O(n log n) 시간, O(1) 공간에 수행해야 한다.

*/


int* heap, heap_size = 0;

void buildHeap() {
	heap = (int*)malloc(sizeof(int) * (heap_size + 1));
	heap[0] = 0;
}
void swapElement(int* a, int* b) {
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
void DownHeap(int i) {
	int greater;
	if (2 * i > heap_size)
		return;

	greater = 2 * i;
	if (greater + 1 <= heap_size) {
		if (heap[greater] < heap[2 * i + 1])
			greater = 2 * i + 1;
	}

	if (heap[i] >= heap[greater])
		return;

	swapElement(&heap[i], &heap[greater]);
	DownHeap(greater);
}
void rBuildHeap(int i) {
	if (i > heap_size)
		return;
	rBuildHeap(2 * i);
	rBuildHeap(2 * i + 1);
	DownHeap(i);
}
void inPlaceHeapSort(int n) {
	for (int i = n; i > 1; i--) { // for i <- n down to 2
		swapElement(&heap[1], &heap[i]);
		heap_size--;
		DownHeap(1);
	}
	return;
}
void printArray() {
	for (int i = 1; i <= heap_size; i++)
		printf(" %d", heap[i]);
	printf("\n");
}
int main(void) {
	int key_cnt, key, last;

	scanf("%d", &key_cnt);
	heap_size = key_cnt;
	buildHeap();

	// phase 1 : make heap using recursive heapSort;
	for (int i = 0; i < key_cnt; i++) {
		scanf("%d", &key);
		heap[i + 1] = key;
	}
	rBuildHeap(1);

	// phase 2 : inplaceHeapSrot(
	last = heap_size;
	inPlaceHeapSort(last);
	heap_size = last;
	printArray();

	free(heap);
	return 0;
}