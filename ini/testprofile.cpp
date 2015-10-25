#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
using namespace std;

int main()
{
	TCHAR temppath[MAX_PATH] = {0, };
	DWORD dwTempLength = GetTempPath(MAX_PATH, temppath);

	TCHAR filename[MAX_PATH] = {0, }; 
	_tcscpy(filename, temppath);
	_tcscat(filename, _T("\\moblieposbell.ini"));

	TCHAR section1[] = _T("user");
	TCHAR section2[] = _T("config");
	TCHAR section3[] = _T("server");

	TCHAR key1_1[] = _T("userid");
	TCHAR key1_2[] = _T("userpw");
	TCHAR key1_3[] = _T("autologin");
	TCHAR key1_4[] = _T("saveid");
	TCHAR key1_5[] = _T("status");

	TCHAR key2_1[] = _T("port");
	TCHAR key2_2[] = _T("baudrate");
	TCHAR key2_3[] = _T("calltype");
	TCHAR key2_4[] = _T("refreshtime");
	TCHAR key2_5[] = _T("virtualport");

	TCHAR val1_1[] = _T("kimkisun");
	TCHAR val1_2[] = _T("kimkisun");
	TCHAR val1_3[] = _T("true");
	TCHAR val1_4[] = _T("true");
	TCHAR val1_5[] = _T("logon");

	TCHAR val2_1[] = _T("COM3");
	TCHAR val2_2[] = _T("9600");
	TCHAR val2_3[] = _T("¼ø¹ø");
	TCHAR val2_4[] = _T("30");
	TCHAR val2_5[] = _T("COM6");

	TCHAR buff1[256] = {0, };

	WritePrivateProfileString(section1, key1_1, val1_1, filename);
	WritePrivateProfileString(section1, key1_2, val1_2, filename);
	WritePrivateProfileString(section1, key1_3, val1_3, filename);
	WritePrivateProfileString(section1, key1_4, val1_4, filename);

	WritePrivateProfileString(section2, key2_1, val2_1, filename);
	WritePrivateProfileString(section2, key2_2, val2_2, filename);
	WritePrivateProfileString(section2, key2_3, val2_3, filename);
	WritePrivateProfileString(section2, key2_4, val2_4, filename);
	WritePrivateProfileString(section2, key2_5, val2_5, filename);
	
	GetPrivateProfileString(section1, key1_1, _T(""), buff1, 256, filename);
	//GetPrivateProfileString(section, key, "", temp, 2048, FileName);

	_tprintf(_T("get key: %s\n"), buff1);

	return 0;
}
