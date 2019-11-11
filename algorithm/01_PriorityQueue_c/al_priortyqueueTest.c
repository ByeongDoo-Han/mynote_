#include "selective_n_insertion_header.h"

/*
◦ 작성해야 할 프로그램
① 정렬할 원소의 개수 n을 표준입력 받고, 크기가 n인 정수 배열 A와 B를 동적할당 받는다.
② 난수발생 함수(srand, rand 등)를 사용하여 n개의 정수 난수로 배열 A와 B를 동일하게 초기화한다.
③ 배열 A에 대해서는 선택 정렬을, 배열 B에 대해서는 삽입 정렬을 수행하고, 시간측정 함수(clock 등)를 이용하여 각 정렬에 소요된 시간을 표준출력한다.

◦ 실행시간 비교 분석
다음과 같이 다양한 입력 데이터에 대해 두 정렬 알고리즘의 시간을 측정하여, 두 정렬의 성능을 비교 분석해보자.

A. 각 정렬의 입력으로 정렬이 안 된 데이터가 주어지는 경우
	a) 동일한 n으로 여러 번 실험하여, 어느 정렬이 더 빠른지 비교해보자.
	b) n이 증가함에 따라 두 정렬의 실행시간이 어떤 비율로 증가하는지 확인해보자.

B. 각 정렬의 입력으로 정렬된 데이터가 주어지는 경우.
※ 참고: 정렬된 데이터로 실험하기 위해서는 ②번과 ③번 사이에, A와 B를 아무 정렬 알고리즘이나 사용해서 정렬시키는 과정을 추가하면 된다(시간은 ③에서 측정).
	a) 동일한 n으로 여러번 실험하여, 어느 정렬이 더 빠른지 비교해보자.
	b) n이 증가함에 따라 두 정렬의 실행시간이 어떤 비율로 증가하는지 확인해보자.

C. 각 정렬의 입력으로 역순으로 정렬된 데이터가 주어지는 경우.
※ 참고: 정렬된 데이터로 실험하기 위해서는 ②번과 ③번 사이에, A와 B를 아무 정렬 알고리즘이나 사용해서 역순으로 정렬시키는 과정을 추가하면 된다(시간은 ③에서 측정).
	a) 동일한 n으로 여러 번 실험하여, 어느 정렬이 더 빠른지 비교해보자.
	b) n이 증가함에 따라 두 정렬의 실행시간이 어떤 비율로 증가하는지 확인해보자.

*/

int* sel_min(int* arr, int size) {
	int* p = arr, * min = arr;
	for (; p < arr + size; p++) {
		if (*min > * p) {
			min = p;
		}
	}
	return min;
}

void R_selectiveSort(int* arr, int* queue, int size) {
	int* min;
	min = sel_min(arr, size);
	swap(min, queue);
}

void print_array(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main(void) {
	int* arr_A, * arr_B, n, input, *p, size;
	LARGE_INTEGER ticksPerSec;
	LARGE_INTEGER start, end, diff;
	
	scanf("%d", &n);
	arr_A = (int*)malloc(sizeof(int) * n);
	arr_B = (int*)malloc(sizeof(int) * n);

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		input = rand();
		arr_A[i] = input;
		arr_B[i] = input;
	}
	QueryPerformanceFrequency(&ticksPerSec);
	/*print_array(arr_A, n);
	print_array(arr_B, n);*/
	//print_start_end(arr_A, arr_B, n);

	// A 정렬이 안된 데이터
	// 선택
	printf("A 정렬이 안된 데이터\n");
	QueryPerformanceCounter(&start);

	selective_sort(arr_A, n);

	QueryPerformanceCounter(&end);
	diff.QuadPart = end.QuadPart - start.QuadPart;
	printf("selective sort : %.12fms\n", ((double)diff.QuadPart) / (double)ticksPerSec.QuadPart);

	// 삽입
	QueryPerformanceCounter(&start);

	insertion_sort(arr_B, n);

	QueryPerformanceCounter(&end);
	diff.QuadPart = end.QuadPart - start.QuadPart;
	printf("insertion sort : %.12fms\n", ((double)diff.QuadPart) / (double)ticksPerSec.QuadPart);

	printf("\n\n");


	// B 입력으로 정렬된 데이터가 주어진경우
	printf("B 입력으로 정렬된 데이터가 주어진경우\n");
	// 배열 정렬 후
	// 선택
	QueryPerformanceCounter(&start);

	selective_sort(arr_A, n);

	QueryPerformanceCounter(&end);
	diff.QuadPart = end.QuadPart - start.QuadPart;
	printf("selective sort : %.12fms\n", ((double)diff.QuadPart) / (double)ticksPerSec.QuadPart);

	// 삽입
	QueryPerformanceCounter(&start);

	insertion_sort(arr_B, n);

	QueryPerformanceCounter(&end);
	diff.QuadPart = end.QuadPart - start.QuadPart;
	printf("insertion sort : %.12fms\n", ((double)diff.QuadPart) / (double)ticksPerSec.QuadPart);
	printf("\n\n");


	// C 입력으로 역순으로 정렬된 데이터가 주어지는 경우
	printf("C 입력으로 역순 데이터\n");
	// 역순 정렬
	p = (arr_A + n - 1);
	size = n;
	for (int i = 0; i < n; i++) {
		R_selectiveSort(arr_A, p, size);
		p--;
		size--;
	}

	p = (arr_B + n - 1);
	size = n;
	for (int i = 0; i < n; i++) {
		R_selectiveSort(arr_B, p, size);
		p--;
		size--;
	}

	// 선택
	QueryPerformanceCounter(&start);

	selective_sort(arr_A, n);

	QueryPerformanceCounter(&end);
	diff.QuadPart = end.QuadPart - start.QuadPart;
	printf("selective sort : %.12fms\n", ((double)diff.QuadPart) / (double)ticksPerSec.QuadPart);

	// 삽입
	QueryPerformanceCounter(&start);

	insertion_sort(arr_B, n);

	QueryPerformanceCounter(&end);
	diff.QuadPart = end.QuadPart - start.QuadPart;
	printf("insertion sort : %.12fms\n", ((double)diff.QuadPart) / (double)ticksPerSec.QuadPart);
}