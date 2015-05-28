#include <windows.h>
#include <Shlobj.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
using namespace std;

#pragma comment(lib, "Shell32.lib")

#include "SafeHardHSM.h"


struct ck_c_initialize_args {
	void* create;
	void* destroy;
	void* lock;
	void* unlock;
	int flags;
	void* reserved;
};

int main()
{
	TCHAR tszPath[MAX_PATH] = {0, };
	TCHAR tszModulePath[MAX_PATH] = {0, };
	string modulePath = "";

	HRESULT hr = SHGetSpecialFolderPath(NULL, tszPath, CSIDL_PROGRAM_FILES, TRUE);
	if (!SUCCEEDED(hr)) {
		printf("fail: get folder...\n");
	} else {
		_tprintf(_T("program files folder: [%s]\n"), tszPath);
		_tcscat_s(tszPath, MAX_PATH, _T("\\wizvera\\safehard\\SafehardHSM.dll"));
		_tprintf(_T("module path: [%s]\n"), tszPath);
	}

	SAFEHARD_ARGS sf_args;



	return 0;
}

