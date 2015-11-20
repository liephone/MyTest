
#include <windows.h>
#include <windef.h>
#include <Dbt.h>
#include <io.h>
#include <errno.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#include <Shellapi.h>
#pragma comment(lib, "Shell32.lib")

#include <string>
#include <vector>
using namespace std;

#define DRIVE_NUM       23

// GetLogicalDrives 함수의 return 값 bitmask : (A, B, C, 제외)
const DWORD drivemask[DRIVE_NUM + 1] = { 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16383, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432 };
const WCHAR drivechar[DRIVE_NUM + 1] = { L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K', L'L', L'M', L'N', L'O', L'P', L'Q', L'R', L'S', L'T', L'U', L'V', L'W', L'X', L'Y', L'Z' };

char* ConvertUnicodeToMultibyte(wchar_t* wszSrc);


int main()
{

	vector<WCHAR> letters;

	FILE* pFile = _wfopen(L"out.txt", L"a+");
	if (pFile == NULL) {
		printf("cannot open file !!!\n");
		return 1;
	}

    DWORD dwList = GetLogicalDrives();
    for (int i = 0; i < DRIVE_NUM + 1; i++) {
        if ((dwList & drivemask[i]) != drivemask[i]) {
            letters.push_back(drivechar[i]);
			//wprintf(L"%c ", drivechar[i]);
		} else {
			wprintf(L"%c ", drivechar[i]);
			fwprintf(pFile, L"%c ", drivechar[i]);

			WCHAR szRootpath[5] = {0, };
			_swprintf(szRootpath, L"%c:\\", drivechar[i]);

			SHFILEINFOW sfi;
			memset(&sfi, 0, sizeof(sfi));
			UINT uFlags = SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX |SHGFI_DISPLAYNAME;
			DWORD retFileInfo = SHGetFileInfoW(szRootpath, 0, &sfi, sizeof(SHFILEINFO), uFlags);
			char* name = ConvertUnicodeToMultibyte(sfi.szDisplayName);

            int type = GetDriveTypeW(szRootpath);
			if (type == DRIVE_REMOVABLE) {
				wprintf(L": removable drive\n");
				wprintf(L"[%s]\n", sfi.szDisplayName);

				fprintf(pFile, ": removable drive\n");
				fprintf(pFile, "[%s]\n", name);

			} else if (type == DRIVE_FIXED) {
				wprintf(L": fixed drive\n");
				wprintf(L"[%s]\n", sfi.szDisplayName);
				fprintf(pFile, ": fixed drive\n");
				fprintf(pFile, "[%s]\n", name);

			} else if (type == DRIVE_CDROM) {
				wprintf(L": cdrom drive\n");
				wprintf(L"[%s]\n", sfi.szDisplayName);
				fprintf(pFile, ": cdrom drive\n");
				fprintf(pFile, "[%s]\n", name);
			}

			delete name;
		}
    }

	printf("\n");
	fprintf(pFile, "\n");

	fclose(pFile);

	HANDLE hComm = CreateFileA("LPT1", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hComm == INVALID_HANDLE_VALUE) {
		printf("LPT1: invalid port\n");
	} else {
		printf("LPT1: valid port\n");
	}

	hComm = CreateFileA("LPT2", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hComm == INVALID_HANDLE_VALUE) {
		printf("LPT2: invalid port\n");
	} else {
		printf("LPT2: valid port\n");
	}

	/*
    if (!letters.empty()) {
        wstring drive = L"";
        WCHAR ch = letters.at(0);
        drive.push_back(ch);
        drive.push_back(L':');
        return drive;
    } else {
        return L"";
    }
	*/

	return 0;
}


char* ConvertUnicodeToMultibyte(wchar_t* wszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, wszSrc, -1, NULL, 0, NULL, NULL);

	char* pszDest = new char[nLen];
	memset(pszDest, 0x00, (nLen) * sizeof(char));

	WideCharToMultiByte(CP_ACP, 0, wszSrc, -1, pszDest, nLen, NULL, NULL);

	return pszDest;
}

