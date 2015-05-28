#include <windows.h>
#include <tchar.h>
#include <wchar.h>
#include <stdio.h>
 

// If the function succeeds, the return value is nonzero.
BOOL GetVolNameByDriverName(wchar_t drive, wchar_t* lpszVolumeName, DWORD cchBufferLength)
{
	BOOL bRet = FALSE;
	wchar_t szVolumeMountPoint[3+1] = {0, };

	//wsprintf(szVolumeMountPoint, TEXT("%c:\\"), drive);
	swprintf(szVolumeMountPoint, L"%c:\\", drive);

	bRet = GetVolumeNameForVolumeMountPointW(szVolumeMountPoint, (LPWSTR)lpszVolumeName, cchBufferLength);

	wprintf(L"szVolumeMountPoint: %s\n", szVolumeMountPoint);
	wprintf(L"lpszVolumeName: %s\n", lpszVolumeName);

	return bRet;
}


void _tmain(int argc, TCHAR *argv[])
{
	wchar_t volName[MAX_PATH] = {0, };

	BOOL bSuccess = GetVolNameByDriverName(L'I', volName, MAX_PATH);
	if(bSuccess) {
		wprintf(L"%s\n", volName);
	} else {
		wprintf(L"Error : %d\n", GetLastError());
	}
}
 
