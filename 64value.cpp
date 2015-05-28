#include <windows.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
	unsigned long long i64Value = 0;

	printf("int size : %d\n", sizeof(int));
	printf("i64 size : %d\n", sizeof(int64_t));
	printf("ull size : %d\n", sizeof(unsigned long long));

	return 0;
}

