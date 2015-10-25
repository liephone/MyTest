#include <stdio.h>
#include <string>
using namespace std;

int main()
{
	// Partial cut
	char cmd_cut[] = { 0x1B, 0x69 };

	printf("Partial cut : %d %d\n", 0x1B, 0x69);
	printf("Partial cut : %d %d\n", 0x1B, 0x6D);
	printf("Select cut mode : %d %d\n", 0x1D, 0x56);
	printf("cmd cut : 0x%X 0x%X 0x%X 0x%X\n", 29, 86, 66, 6);
	printf("char: a(0x%X) !(0x%X) d(0x%X) V(0x%X) @(0x%X)\n", 'a', '~', 'd', 'V', '@');
	printf("char: (0x%X) 10(0x%X) 27(0x%X) 29(0x%X) V(0x%X) 1(0x%X)\n", ' ', 10, 27, 29, 'V', 1);
	printf("char: h(0x%X) w(0x%X) H(0x%X) k(0x%X) d(0x%X) V(0x%X)\n", 'h', 'w', 'H', 'k', 'd', 'V');
	printf("char: 50(0x%X) 70(0x%X) 3(0x%X) 0(0x%X) 65(0x%X) 13(0x%X) %d\n", 50, 70, 3, 0, 65, 13, 0xD);

	return 0;
}

