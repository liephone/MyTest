#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void dbPrint(int n, ...);

//#define NUMARGS(...)  (sizeof((int[]) {__VA_ARGS__}) / sizeof(int))
//#define DDbgPrint(...) { printf("[LOG] " __VA_ARGS__); }

#define DDbgPrint(int n, ...) { char* args[] = { NULL, __VA_ARGS__, NULL }; dbPrint(args + 1); }

/*
va_start(va_list list) ;
va_arg(va_list list, type) ;
va_end(va_list list );
*/


int main()
{
	int i = 5;

	DDbgPrint("test....%d%n", i);

	//printf("typeof : %d %d\n", typeid(i), typeid(ar));

	return 0;
}

void dbPrint(int n, ...)
{
	int i;
	int val;

	va_list vl;
	va_start(vl, n);

	for (int i = 0; i < n; i++) {
		val = va_arg(vl, int);

	}
}

