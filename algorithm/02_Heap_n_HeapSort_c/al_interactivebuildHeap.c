#include <stdio.h>
/*
[상향식 힙 생성 & 제자리 힘정렬 공통사항]

다음 문제 1과 2는 각각 삽입식 및 상향식 두 가지 버전의 힙 생성에 대한 프로그래밍 요구다. 다음은 두 문제 모두에 공통된 요구 사항이다.
1) 순차힙으로 구현한다. 즉, 배열을 이용한 순차트리 형식으로 힙을 저장한다.
2) 최대힙으로 구현한다. 따라서 힙으로부터 삭제 작업은 우선순위 큐에서 일반적으로 이루어지는 최소값 삭제가 아닌 최대값 삭제가 된다(참고로 최소힙에서는 최소값 삭제를 수행함).
3) 연산 효율을 위해 배열 인덱스 0 위치는 사용하지 않고 비워둔다.
4) 데이터구조를 단순화하기 위해 힙의 항목으로써 (키, 원소) 쌍에서 원소를 생략하고 키만 저장하는 것으로 한다.
5) 키들은 중복이 없는 1 이상의 정수로 전제한다 – 즉, 중복 키 검사는 불필요하다.
6) O(1) 공간으로 수행할 것 – 즉, 주어진 키들을 저장한 초기 배열 외 추가 메모리 사용은 O(1)을 초과할 수 없다.
7) 힙은 어느 시점에서라도 최대 항목 수 < 100 으로 전제한다.

[삽입식 힙생성]

- 순차힙으로 구성(배열)
- 최대힙으로 구현


1) 키들은 한 개씩 차례로 삽입 명령과 함께 주어진다. 즉, 키가 입력될 때마다 즉시 힙에 삽입해야 한다. 만약 이렇게 하지 않고 문제 2에서 하는 것처럼 키들이 모두 입력되기를 기다려 한꺼번에 상향식으로 힙을 생성하면 대화식 프로그램의 인쇄(p) 또는 삭제(d) 명령의 수행이 어려워진다.
2) 대화식 프로그램에 주어지는 명령은 i, d, p, q 네 가지며 각 명령에 대해 다음과 같이 수행해야 한다.
i <키> : <키>를 힙에 삽입하고 0을 인쇄.
d : 힙에서 삭제(이때 루트에 저장된 키가 삭제되어야 한다)하여 반환된 키를 인쇄.
p : 힙의 내용을 인쇄(이때 이진트리의 레벨순서로 항목들을 인쇄해야 한다).
* 레벨순서란 이진트리의 레벨 0의 노드부터 다음 레벨 1, 2, 3, ...의 노드들을 차례로 방문한다. 같은 레벨의 노드들은 왼쪽에서 오른쪽으로 방문한다.
q : 프로그램 종료

주의:
	1. 힙 인쇄 명령 p는 힙의 크기가 1 이상인 경우에만 적용한다고 전제한다.

힌트:
	1. 현재 총 항목 수를 유지하는 변수 n을 유지하고 삽입 및 삭제 직후 갱신한다.
	2. 순차힙에 새로운 항목의 초기 삽입 위치는 항상 n + 1이다.

*/

int arr[100] = { 0, }, size;

void DownHeap(int * node)
{
	int index = node - arr, *greater, tmp;
	if (index >= size)
		return;
	greater = &arr[2 * index];
	if (*greater < arr[2 * index + 1])
		greater = &arr[2 * index + 1];

	if (*node < *greater)
	{
		tmp = *node;
		*node = *greater;
		*greater = tmp;
	}
	DownHeap(greater);
}
//void UpHeap(int* node)
//{
//	if (node == &arr[1]) // isRoot
//		return;
//	int index = node - arr, tmp;
//	if (*node > arr[index / 2])
//	{
//		tmp = *node;
//		*node = arr[index / 2];
//		arr[index / 2] = tmp;
//	}
//	UpHeap(&arr[index / 2]);
//}
int insertItem(int item) 
{
	size++;
	arr[size] = item;
	DownHeap(&arr[1]);
	//UpHeap(&arr[size]);
	return 0;
}
int removeMax() // heap의 처음 키값을 반환
{
	int deleted = arr[1];
	arr[1] = arr[size];
	arr[size] = 0;
	size--;
	DownHeap(&arr[1]);
	return deleted;
}
void printHeap()
{
	for (int i = 1; i<= size; i++)
	{
		printf(" %d", arr[i]);
	}
	printf("\n");
}
int main(void)
{

	char command;
	int item;

	while (1)
	{
		scanf("%c", &command);
		switch (command)
		{
		case 'i':
			scanf("%d", &item);
			if (!insertItem(item))
				printf("0\n");
			break;
		case 'd':
			printf("%d\n", removeMax());
			break;
		case 'p':
			printHeap();
			break;
		case 'q':
			return 0;
		}
	}
}
