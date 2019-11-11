#include <stdio.h>
/*
[상향식 힙 생성]

1) 이번엔 키들이 미리 한꺼번에 주어진다. 이들을 주어진 순서대로 초기 배열에 저장한다.
2) 초기 배열에 저장된 키들을 상향식으로 재배치하여 힙을 생성한다. 상향식 힙 생성을 위한 재귀 또는 비재귀 방식의 알고리즘 가운데 어느 전략을 사용해도 좋다(사용하지 않은 나머지 전략도 나중에 작성해보자).
3) 참고로 재귀, 비재귀 두 가지 방식 모두 O(n) 시간에 수행 가능하므로(왜 그런지 복습하자) 그렇게 되도록 작성해야 한다.
*/
int heap[100], heap_size = 0;

void DownHeap(int i)
{
	int greater, tmp;
	if (i >= heap_size)
		return;
	greater = 2 * i;						// greater <- lchild
	if (heap[greater] < heap[2 * i + 1])	// lchild < rchild
		greater = 2 * i + 1;				// greater <- rchild

	if (heap[i] < heap[greater])			// swap
	{
		tmp = heap[i];
		heap[i] = heap[greater];
		heap[greater] = tmp;
	}
	DownHeap(greater);
}
void rBuildHeap(int i) 
{
	if (i > heap_size)
		return;
	rBuildHeap(2 * i);
	rBuildHeap(2 * i + 1);
	DownHeap(i);
	return;
}
void printHeap()
{
	for (int i = 1; i <= heap_size; i++)
	{
		printf(" %d", heap[i]);
	}
	printf("\n");
}

int main() {
	int key_cnt, key;

	scanf("%d", &key_cnt);

	for (int i = 1; i <= key_cnt; i++)
	{
		scanf("%d", &key);
		heap[i] = key;
		heap_size++;
	}
	rBuildHeap(1);
	printHeap();
}