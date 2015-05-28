#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <string.h>

#pragma comment(lib, "Advapi32.lib")

#define UNICODE
#define _UNICODE

BOOL RegCheckNode(HKEY hKeyRoot, LPTSTR lpSubKey);
BOOL RegCreateNode(HKEY* phKey, HKEY hKeyRoot, LPTSTR lpSubKey);
BOOL RegSetValueString(HKEY* phKey, LPCTSTR lpValue, LPCTSTR lpData);
BOOL RegSetValueDword(HKEY* phKey, LPCTSTR lpValue, DWORD dwData);
BOOL RegSetValueQword(HKEY* phKey, LPCTSTR lpValue, long long llData);
BOOL RegDeleteNodeRecursive(HKEY hKeyRoot, LPTSTR lpSubKey);
BOOL RegDeleteNode(HKEY hKeyRoot, LPTSTR lpSubKey);


int main()
{
	HKEY hKey;
	
	// HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logoff
	if (RegCheckNode(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff")) == FALSE) {
		RegCreateNode(&hKey, HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff"));
		RegCreateNode(&hKey, HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logon"));
	}

	// Logoff - 0
	if (RegCheckNode(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff\\0")) == FALSE) {
		RegCreateNode(&hKey, HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff\\0"));
	}
	RegSetValueString(&hKey, TEXT("DisplayName"), TEXT("Local Group Policy"));							//
	RegSetValueString(&hKey, TEXT("FileSysPath"), TEXT("C:\\WINDOWS\\System32\\GroupPolicy\\User"));
	RegSetValueString(&hKey, TEXT("GPO-ID"), TEXT("LocalGPO"));
	RegSetValueString(&hKey, TEXT("GPOName"), TEXT("Local Group Policy"));
	RegSetValueDword(&hKey, TEXT("PSScriptOrder"), 1);
	RegSetValueString(&hKey, TEXT("SOM-ID"), TEXT("Local"));

	// Logoff - 0 - 0
	if (RegCheckNode(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff\\0\\0")) == FALSE) {
		RegCreateNode(&hKey, HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff\\0\\0"));
	}
	RegSetValueQword(&hKey, TEXT("ExecTime"), 0);
	RegSetValueDword(&hKey, TEXT("IsPowershell"), 0);
	RegSetValueString(&hKey, TEXT("Parameters"), TEXT(""));
	RegSetValueString(&hKey, TEXT("Script"), TEXT("safehardtray.bat"));


	// S-1-5-21-532101069-4145159380-969375416-1001

	// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\State\S-1-5-21-532101069-4145159380-969375416-1001\Scripts\Logoff
	// \0
	// - DisplayName : 로컬 그룹 정책
	// - FileSysPath : C:\WINDOWS\System32\GroupPolicy\User
	// - GPO-ID : LocalGPO
	// - GPOName : 로컬 그룹 정책
	// - PSScriptOrder : 1
	// - SOM-ID : Local
	
	// \0\0
	// - ExecTime : 0
	// - Parameters : 
	// - Script : safehardtray.bat

	if (RegCheckNode(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\S-1-5-21-532101069-4145159380-969375416-1001\\Scripts\\Logoff")) == FALSE) {
		RegCreateNode(&hKey, HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\S-1-5-21-532101069-4145159380-969375416-1001\\Scripts\\Logoff"));
		RegCreateNode(&hKey, HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\S-1-5-21-532101069-4145159380-969375416-1001\\Scripts\\Logon"));
	}

	// Logoff - 0
	if (RegCheckNode(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\S-1-5-21-532101069-4145159380-969375416-1001\\Scripts\\Logoff\\0")) == FALSE) {
		RegCreateNode(&hKey, HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\S-1-5-21-532101069-4145159380-969375416-1001\\Scripts\\Logoff\\0"));
	}
	RegSetValueString(&hKey, TEXT("DisplayName"), TEXT("Local Group Policy"));							//
	RegSetValueString(&hKey, TEXT("FileSysPath"), TEXT("C:\\WINDOWS\\System32\\GroupPolicy\\User"));
	RegSetValueString(&hKey, TEXT("GPO-ID"), TEXT("LocalGPO"));
	RegSetValueString(&hKey, TEXT("GPOName"), TEXT("Local Group Policy"));
	RegSetValueDword(&hKey, TEXT("PSScriptOrder"), 1);
	RegSetValueString(&hKey, TEXT("SOM-ID"), TEXT("Local"));

	// Logoff - 0 - 0
	if (RegCheckNode(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\S-1-5-21-532101069-4145159380-969375416-1001\\Scripts\\Logoff\\0\\0")) == FALSE) {
		RegCreateNode(&hKey, HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\S-1-5-21-532101069-4145159380-969375416-1001\\Scripts\\Logoff\\0\\0"));
	}
	RegSetValueQword(&hKey, TEXT("ExecTime"), 0);
	RegSetValueString(&hKey, TEXT("Parameters"), TEXT(""));
	RegSetValueString(&hKey, TEXT("Script"), TEXT("safehardtray.bat"));

	return 0;
}


BOOL RegCheckNode(HKEY hKeyRoot, LPTSTR lpSubKey)
{
	LONG lResult = 0;
	HKEY hKey;

	lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	RegCloseKey(hKey);
	if (lResult == ERROR_SUCCESS) {
		printf("RegCheckNode:: open key complete\n");
		return TRUE;
	}
	else {
		printf("RegCheckNode:: open key failed\n");
		return FALSE;
	}
}


BOOL RegCreateNode(HKEY* phKey, HKEY hKeyRoot, LPTSTR lpSubKey)
{
	LONG lResult = 0;

	lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, phKey);
	if (lResult == ERROR_SUCCESS) {
		printf("RegCreateNode:: open key complete\n");
		return TRUE;
	}

	lResult = RegCreateKeyEx(hKeyRoot,
		lpSubKey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		phKey,
		NULL);
	if (lResult == ERROR_SUCCESS) {
		printf("RegCreateNode:: create key complete\n");
		return TRUE;
	}

	return FALSE;
}


BOOL RegSetValueString(HKEY* phKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	LONG lResult = 0;

	lResult = RegSetValueEx(*phKey,
		lpValue,
		0,
		REG_SZ,
		(const BYTE *)lpData,
		_tcslen(lpData) * sizeof(TCHAR) + 1);	// _tcslen 함수로 할 경우 console build 에서는 오류.
	if (lResult == ERROR_SUCCESS) {
		printf("RegSetValueS:: set key(s) complete\n");
		return TRUE;
	}
	else {
		printf("RegSetValueS:: set key(s) failed\n");
		return FALSE;
	}

}


BOOL RegSetValueDword(HKEY* phKey, LPCTSTR lpValue, DWORD dwData)
{
	LONG lResult = 0;

	lResult = RegSetValueEx(*phKey,
		lpValue,
		0,
		REG_DWORD,
		(LPBYTE)&dwData,
		sizeof(dwData));
	if (lResult == ERROR_SUCCESS) {
		printf("RegSetValueD:: set key(d) complete\n");
		return TRUE;
	}
	else {
		printf("RegSetValueD:: set key(d) failed\n");
		return FALSE;
	}
}


BOOL RegSetValueQword(HKEY* phKey, LPCTSTR lpValue, long long llData)
{
	LONG lResult = 0;

	lResult = RegSetValueEx(*phKey,
		lpValue,
		0,
		REG_QWORD,
		(LPBYTE)&llData,
		sizeof(llData));
	if (lResult == ERROR_SUCCESS) {
		printf("RegSetValueD:: set key(d) complete\n");
		return TRUE;
	}
	else {
		printf("RegSetValueD:: set key(d) failed\n");
		return FALSE;
	}
}


BOOL RegDeleteNodeRecursive(HKEY hKeyRoot, LPTSTR lpSubKey)
{
	LPTSTR lpEnd;
	LONG lResult = 0;
	DWORD dwSize = 0;
	TCHAR szName[MAX_PATH] = { 0, };
	HKEY hKey;
	FILETIME ftWrite;

	lResult = RegDeleteKey(hKeyRoot, lpSubKey);
	if (lResult == ERROR_SUCCESS)
		return TRUE;

	lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS)
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("RegDeleteNodeRecursive:: key not found.\n");
			return TRUE;
		}
		else {
			printf("RegDeleteNodeRecursive:: cannot opening key.\n");
			return FALSE;
		}
	}

	// Check for an ending slash and add one if it is missing.
	lpEnd = lpSubKey + lstrlen(lpSubKey);
	if (*(lpEnd - 1) != TEXT('\\'))
	{
		*lpEnd = TEXT('\\');
		lpEnd++;
		*lpEnd = TEXT('\0');
	}

	// Enumerate the keys
	dwSize = MAX_PATH;
	lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
		NULL, NULL, &ftWrite);
	if (lResult == ERROR_SUCCESS)
	{
		do {
			StringCchCopy(lpEnd, MAX_PATH * 2, szName);
			if (!RegDeleteNodeRecursive(hKeyRoot, lpSubKey)) {
				break;
			}
			dwSize = MAX_PATH;
			lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
				NULL, NULL, &ftWrite);
		} while (lResult == ERROR_SUCCESS);
	}
	lpEnd--;
	*lpEnd = TEXT('\0');

	RegCloseKey(hKey);

	// Try again to delete the key.
	lResult = RegDeleteKey(hKeyRoot, lpSubKey);
	if (lResult == ERROR_SUCCESS) {
		printf("RegDeleteNodeRecursive:: retry delete complete.\n");
		return TRUE;
	}
	else {
		printf("RegDeleteNodeRecursive:: retry delete failed.\n");
		return FALSE;
	}
}


BOOL RegDeleteNode(HKEY hKeyRoot, LPTSTR lpSubKey)
{
	TCHAR szDelKey[MAX_PATH * 2] = { 0, };

	StringCchCopy(szDelKey, MAX_PATH * 2, lpSubKey);
	return RegDeleteNodeRecursive(hKeyRoot, szDelKey);
}

