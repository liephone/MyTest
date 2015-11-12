#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main()
{
	wchar_t tszPort[] = L"\\\\.\\COM21";

	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = ::CreateFileW(tszPort,
						   GENERIC_READ|GENERIC_WRITE,
						   0,
						   0,
						   OPEN_EXISTING,
						   FILE_FLAG_OVERLAPPED,
						   0);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("INVALID_HANDLE_VALUE\n");
	} else {
		printf("open success\n");
	}

	::CloseHandle(hFile);

	return 0;
}
