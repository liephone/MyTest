#include <windows.h>
#include <wchar.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#define DRIVE_NUM	26

const DWORD drivemask[DRIVE_NUM + 1] = { 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16383, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432 };
const wchar_t drivechars[DRIVE_NUM + 1] = { L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K', L'L', L'M', L'N', L'O', L'P', L'Q', L'R', L'S', L'T', L'U', L'V', L'W', L'X', L'Y', L'Z' };

BOOL IsSystemDrive(LPCWSTR MountPoint);


int main()
{
	wstring test = L"A:";

	// get
	vector<wchar_t> letters;
    DWORD dwList = GetLogicalDrives();
    for (int i = 0; i < DRIVE_NUM + 1; i++) {
        if ((dwList & drivemask[i]) == drivemask[i])
            letters.push_back(drivechars[i]);
    }

	// print
	for (int i = 0; i < letters.size(); i++) {
		wprintf(L"%c ", letters[i]);
	}
	wprintf(L"\n");

	if (IsSystemDrive(test.c_str()) == TRUE) {
		wprintf(L"%s is a system drive.\n", test.c_str());
	} else {
		wprintf(L"%s is not a system drive.\n", test.c_str());
	}

	return 0;
}

BOOL IsSystemDrive(LPCWSTR MountPoint)
{
	BOOL bRet = FALSE;
	wchar_t wch = MountPoint[0];

	if (wch == L'A' || wch == L'B' || wch == L'C') {
		return TRUE;
	}

	DWORD dwList = GetLogicalDrives();
	for (int i = 0; i < DRIVE_NUM + 1; i++) {
		if ((dwList & drivemask[i]) == drivemask[i]) {
			if (wch == drivechars[i]) {
				bRet = TRUE;
			}
		}
	}

	return bRet;
}
