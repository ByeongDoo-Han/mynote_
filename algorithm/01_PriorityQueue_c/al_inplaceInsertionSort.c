#include <stdio.h>
#include <stdlib.h>
/*
◦ 구현해야 할 삽입 정렬 알고리즘:
- 크기가 n인 배열을 동적 할당하여, 입력된 양의 정수 저장(입력 정수는 중복 가능)
- 제자리(in-place) 정렬 사용.즉, 입력 값 저장을 위한 배열 이외에 O(1)의 추가 공간만 사용
- 배열의 앞부분을 정렬 상태로 유지
- 가능하면 교재의 의사코드를 보지 말고 구현해볼 것을 권장

◦ 알고리즘 동작 과정 예시 (n = 7)
	초기 상태 :
3	73	48	31	8	11	20

	1번째 반복 후:
3	73	48	31	8	11	20 (73 삽입)

	2번째 반복 후:
3	48	73	31	8	11	20 (48 삽입)

	3번째 반복 후:
3	31	48	73	8	11	20 (31 삽입)

	4번째 반복 후:
3	8	31	48	73	11	20 (8 삽입)

	5번째 반복 후:
3	8	11	31	48	73	20 (11 삽입)

	6번째 반복 후:
3	8	11	20	31	48	73 (20 삽입)
*/

void insertion_sort(int* arr, int size) {
	int save, j;
	
	for (int i = 1; i < size; i++) {
		save = arr[i];
		j = i - 1;
		while ((j >= 0) && (arr[j] > save)) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = save;
	}

	return;
}

int main(void) {
	int n, * arr;

	scanf("%d", &n);
	arr = (int*)malloc(sizeof(int) * n);

	for (int i = 0; i < n; i++) {
		scanf("%d", &arr[i]);
	}
	
	insertion_sort(arr, n);

	for (int i = 0; i < n; i++) {
		printf(" %d", arr[i]);
	}
	free(arr);
}