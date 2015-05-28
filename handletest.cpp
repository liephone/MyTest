#include <windows.h>
#include <tchar.h>
#include <stdio.h>

void PrintError(LPCTSTR errDesc);

int main()
{

	TCHAR szTempFile[MAX_PATH] = {0, };
	TCHAR szTempDir[] = _T("C:\\Users\\wiz\\AppData\\Local\\Temp\\SafeHard");

	UINT uRetVal = 0;
	uRetVal = GetTempFileName(szTempDir, // directory for tmp files
                              TEXT("DEMO"),     // temp file name prefix 
                              0,                // create unique name 
                              szTempFile);  // buffer for name 
    if (uRetVal == 0) {
        PrintError(TEXT("GetTempFileName failed"));
	} else {
		_tprintf(_T("%s\n"), szTempFile);
	}


	HANDLE hDir = INVALID_HANDLE_VALUE;
	hDir = CreateFile((LPTSTR) szTempDir,        // file name 
                           GENERIC_WRITE,        // open for write 
                           0,                    // do not share 
                           NULL,                 // default security 
                           CREATE_ALWAYS,        // overwrite existing
                           FILE_ATTRIBUTE_NORMAL,// normal file 
                           NULL);                // no template 
    if (hDir == INVALID_HANDLE_VALUE) {
		PrintError(TEXT("CreateFile dir failed"));
	} else {
		_tprintf(_T("handle : %d\n"), hDir);
	}


	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile((LPTSTR) szTempFile,      // file name 
                           GENERIC_WRITE,        // open for write 
                           0,                    // do not share 
                           NULL,                 // default security 
                           CREATE_ALWAYS,        // overwrite existing
                           FILE_ATTRIBUTE_NORMAL,// normal file 
                           NULL);                // no template 
    if (hFile == INVALID_HANDLE_VALUE) {
		PrintError(TEXT("CreateFile file failed"));
	} else {
		_tprintf(_T("handle : %d\n"), hFile);
	}



	return 0;
}


LPCTSTR ErrorMessage(DWORD error) 
{ 
    LPVOID lpMsgBuf;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER 
                   | FORMAT_MESSAGE_FROM_SYSTEM 
                   | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  error,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0,
                  NULL);

    return((LPCTSTR)lpMsgBuf);
}

void PrintError(LPCTSTR errDesc)
{
        LPCTSTR errMsg = ErrorMessage(GetLastError());
        _tprintf(TEXT("\n** ERROR ** %s: %s\n"), errDesc, errMsg);
        LocalFree((LPVOID)errMsg);
}

