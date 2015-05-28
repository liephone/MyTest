#include <windows.h>
#include <stdio.h>

int main()
{
	double db = 99999999999999.0;

	printf("double: %lf, int: %d\n", db, db);

	if (db > 1000.0) {
		printf("bigger\n");
	} else {
		printf("smaller\n");
	}

	return 0;
}
