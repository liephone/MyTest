#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "killchrome.lib")

extern "C" __declspec(dllexport) int __cdecl killmain();

int main()
{
	printf("kill....\n");

	killmain();

	return 0;
}

