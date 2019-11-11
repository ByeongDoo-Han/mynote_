#include <stdio.h>
#include <stdlib.h>

/*
◦ 구현해야 할 선택 정렬 알고리즘 (가장 큰 값을 찾는 버전):
- 크기가 n인 배열을 동적 할당하여, 입력된 양의 정수 저장(입력 정수는 중복 가능)
- 제자리(in place) 정렬 사용.즉, 입력 값 저장을 위한 배열 이외에 O(1)의 추가 공간만 사용
- 배열의 뒷 부분을 정렬 상태로 유지하고, 매 반복마다 최대 한 번의 교환 연산만 사용(매 반복마다 가장 큰 값을 찾아, 오른쪽부터 채우는 방식으로 정렬)
- 가능하면 교재의 의사코드를 보지 말고 구현해볼 것을 권장
- 참고: 아래 그림에 예시된 버전은 매 반복마다 가장 작은 값을 찾아 왼쪽부터 채워 나가는, 따라서 교재의 알고리즘과는 정반대 방향으로 작동하는 버전이다.

◦ 알고리즘 동작 과정 예시 (n = 8)
	초기 상태:
8	31	48	73	3	65	20	29

	1번째 반복 후:
8	31	48	29	3	65	20	73 (73과 29교환)

	2번째 반복 후:
8	31	48	29	3	20	65	73 (65와 20교환)

	3번째 반복 후:
8	31	20	29	3	48	65	73 (48과 20교환)

	4번째 반복 후:
8	3	20	29	31	48	65	73 (31과 3교환)

	5번째 반복 후:
8	3	20	29	31	48	65	73 (29제자리 교환)

	6번째 반복 후:
8	3	20	29	31	48	65	73 (20제자리 교환)

	7번째 반복 후:
3	8	20	29	31	48	65	73 (8과 3교환)
*/

void swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int* select_max(int* arr, int size) {
	int* p = arr, *max = arr;
	for (; p < arr + size; p++) {
		if (*max < *p) {
			max = p;
		}
	}
	return max;
}

void inplaceSelectiveSort(int* arr, int size) {
	int* p = arr + size - 1;	// 배열의 뒤부터 fence를 만듦
	int* maxLoc;

	for (int i = 0; i < size - 1; i++) {
		maxLoc = select_max(arr, size - i);	// 0 ~ size - i 중 큰 값
		swap(p, maxLoc);
		p--;
	}
	return;
}

int main(void) {
	int n, *arr;				// natural integer : n (overlapped)

	scanf("%d", &n);
	arr = (int*)malloc(sizeof(int) * n);

	for (int i = 0; i < n; i++) {
		scanf("%d", &arr[i]);
	}

	// inplaceSelectiveSort
	inplaceSelectiveSort(arr, n);

	for (int i = 0; i < n; i++) {
		printf(" %d", arr[i]);
	}
	
	free(arr);
}