#include <windows.h>
#include <tchar.h>
#include <shellapi.h>

bool DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true)
{
	int len = _tcslen(lpszDir);
	TCHAR *pszFrom = new TCHAR[len+2];
	_tcscpy(pszFrom, lpszDir);
	pszFrom[len] = 0;
	pszFrom[len+1] = 0;

	SHFILEOPSTRUCT fileop;
	fileop.hwnd   = NULL;    // no status display
	fileop.wFunc  = FO_DELETE;  // delete operation
	fileop.pFrom  = pszFrom;  // source file name as double null terminated string
	fileop.pTo    = NULL;    // no destination needed
	fileop.fFlags = FOF_NOCONFIRMATION|FOF_SILENT;  // do not prompt the user

	if(!noRecycleBin)
	fileop.fFlags |= FOF_ALLOWUNDO;

	fileop.fAnyOperationsAborted = FALSE;
	fileop.lpszProgressTitle     = NULL;
	fileop.hNameMappings         = NULL;

	int ret = SHFileOperation(&fileop);
	delete [] pszFrom;  
	return (ret == 0);
}

int main() 
{
	DeleteDirectory("C:\\Users\\MyTest\\AppData\\Local\\Temp\\sh_root_r", false);
	return 0;
}
