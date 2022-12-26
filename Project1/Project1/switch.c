#include <stdio.h>

<<<<<<< HEAD
void main(void) {
	int a, b, c = 0;
	a = ++c;
	b = c++;
	printf("%d %d %d\n", a, b, ++c);
=======
int main(void) {
	int a = 1;

	if (a == 1) {
		printf("apple");
	}
	else if (a == 2) {
		printf("banana");
	}
	else{
		printf("mango");
	}

	return 0;
>>>>>>> b3f806dd62302be118bfcd2062d34ec883248954
}