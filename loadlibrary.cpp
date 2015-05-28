#include <afx.h>
#include <Shlobj.h>
#include <iostream>
using namespace std;


int main()
{
	TCHAR tszProgramFiles[MAX_PATH] = {0, };

	HRESULT hr = SHGetSpecialFolderPath(NULL, tszProgramFiles, CSIDL_PROGRAM_FILES, TRUE);
	if (!SUCCEEDED(hr)) {
		// fail: get program file folder
		return FALSE;
	}

	CString strDllPath = _T("SafeHardHSM.dll");
	//strDllPath.Format(_T("%s\\wizvera\\safehard\\SafeHardHSM.dll"), tszProgramFiles);
	//strDllPath.Format(_T("%s\\wizvera\\delfino\\npdelfinoplugin.dll"), tszProgramFiles);
	//strDllPath.Format(_T("%s\\wizvera\\safehard\\pthreadVC2.dll"), tszProgramFiles);

	cout << strDllPath << endl;

	HMODULE hm = LoadLibrary((LPCTSTR)strDllPath);
	if (hm == NULL) {
		cout << _T("LoadLibrary failed..") << endl;
		return FALSE;
	}

	cout << _T("success.") << endl;
	

	return 0;
}

