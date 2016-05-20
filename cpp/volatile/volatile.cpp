#include <stdio.h>
int main()
{
	int i = 10;
	int a = i;
	printf("i = %d", a);

	asm("mov $0x15, %ebx\n\r");

	int b = i;
	printf("i = %d", b);

	return 0;
}
