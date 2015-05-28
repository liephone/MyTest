#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

void _tmain(int argc, TCHAR* argv[])
{
	wchar_t szFileName[] = L"C:\\Users\\wiz\\AppData\\Local\\Temp\\sh_temp_r\\NPKI\\yessign\\USER\\cn=김기선(Kim Ki Sun)0004042702050694,ou=KMB,ou=personal4IB,o=yessign,c=kr\\signCert.der";

	DWORD dwAttrs = GetFileAttributesW(szFileName);
	if (dwAttrs == INVALID_FILE_ATTRIBUTES) {
		wprintf(L"invalid file attributes !!\n");
	} else {
		SetFileAttributesW(szFileName, dwAttrs | FILE_FLAG_DELETE_ON_CLOSE);
		wprintf(L"set attributes successed..\n");
	}


	char szFileName2[] = "C:\\Users\\wiz\\signCert.der";

	DWORD dwAttrs2 = GetFileAttributes(szFileName2);
	if (dwAttrs2 == INVALID_FILE_ATTRIBUTES) {
		printf("invalid file attributes !!\n");
	} else {
		SetFileAttributes(szFileName2, dwAttrs2 | FILE_FLAG_DELETE_ON_CLOSE);
		printf("set attributes successed..\n");
	}
}
