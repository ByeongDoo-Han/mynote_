#include <stdio.h>
#include <stdlib.h>

int BinarySearch(int a, int b, int q_cnt) {
	char ch;
		
	for (int i = 0; i <= q_cnt; i++) {
		int mid = (a + b) / 2;

		ch = getchar();
		if (a == b) { return b; }
		else if (ch == 'Y')	{ a = mid + 1; }
		else if (ch == 'N') { b = mid; }
	}
}

int main(void) {
	int a, b, q_cnt; // a ~ b, count of question

	scanf("%d %d %d", &a, &b, &q_cnt);
	while (getchar() != '\n');
	
	printf("%d", BinarySearch(a, b, q_cnt));

	return 0;
}