#include <stdio.h>

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
int main(void)
{
	int a = 3; 
	int b = 5;

	printf("a : %d b : %d \n", a, b);

	//a�� b�� swap�ϴ� �Լ� ȣ��
	swap(&a, &b);
	printf("a : %d b : %d \n", a, b);
}