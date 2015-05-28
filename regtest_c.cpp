#include <windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <wchar.h>
#include <strsafe.h>
#include <Shlobj.h>

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Shell32.lib")

const int SUBCOUNT = 7;
const int SUBSIZE = 256;
struct _State_Subs {
	WCHAR subs[SUBCOUNT][SUBSIZE];
};
typedef _State_Subs State_Subs;

BOOL InstallScript();
BOOL UninstallScript();

BOOL GetGroupPolicyPath(WCHAR* path, int size);
BOOL GetSafeHardTrayPath(WCHAR* path, int size);
BOOL FindStateLogoffPath(WCHAR* path, int size);

BOOL RegDeleteNode(HKEY hKeyRoot, LPCWSTR lpSubKey);
BOOL RegDeleteNodeRecursive(HKEY hKeyRoot, LPCWSTR lpSubKey);

BOOL RegCreateNode(HKEY* phKey, HKEY hKeyRoot, LPCWSTR lpSubKey);
BOOL RegSetValueString(HKEY* phKey, LPCWSTR lpValue, LPCWSTR lpData);
BOOL RegSetValueDword(HKEY* phKey, LPCWSTR lpValue, DWORD dwData);

BOOL RegCheckNode(HKEY hKeyRoot, LPCWSTR lpSubKey);

DWORD RegGetSubKeyCount(HKEY hKeyRoot, LPCWSTR lpSubKey);
BOOL RegGetKeyNames(State_Subs* subkeynames, HKEY hKeyRoot, LPCWSTR lpSubKey);

BOOL RegisterShutdownScript(HKEY hKeyRoot, LPCWSTR lpSubKey);
BOOL RegisterLogoffScript(HKEY hKeyRoot, LPCWSTR lpSubKey);

BOOL WriteShutdownScript(LPCWSTR lpFilePath);
BOOL WriteLogoffScript(LPCWSTR lpFilePath);

#define REG_PATH_SHUTDOWN	L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown"
#define REG_PATH_LOGOFF		L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff"
#define REG_PATH_STATE		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State"


int main()
{
	InstallScript();
	//UninstallScript();

	/*
	// test::delete
	BOOL bSuccess = RegDeleteNode(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Dokan");
	if (bSuccess == TRUE) {
		printf("delete success..\n");
	} else {
		printf("delete fail !!!!\n");
	}
	*/

	/*
	HKEY hKey;

	// test::create
	bSuccess = RegCreateNode(&hKey, HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Dokan\\Enum");

	// test::set
	bSuccess = RegSetValueString(&hKey, "TestString", "DokanEnum");
	bSuccess = RegSetValueD(&hKey, "Test", 2);

	RegCloseKey(hKey);
	*/

	return 0;
}


BOOL InstallScript()
{
	// create scripts
	WCHAR shutdownpath[MAX_PATH] = {0, }; 
	GetGroupPolicyPath(shutdownpath, MAX_PATH);
	wcscat_s(shutdownpath, MAX_PATH, L"\\Machine\\Scripts\\Shutdown\\safehardtray.bat");

	WCHAR logoffpath[MAX_PATH] = {0, };
	GetGroupPolicyPath(logoffpath, MAX_PATH);
	wcscat_s(logoffpath, MAX_PATH, L"\\User\\Scripts\\Logoff\\safehardtray.bat");

	HANDLE hFile = INVALID_HANDLE_VALUE;
    PVOID OldValue = NULL;

    if (Wow64DisableWow64FsRedirection(&OldValue)) {

		printf("disable redirection..\n");

		WriteShutdownScript(shutdownpath);
		WriteLogoffScript(logoffpath);

		if (FALSE == Wow64RevertWow64FsRedirection(OldValue)) {
			printf("re-enable redirection fail !!!\n");
		} else {
			printf("re-enable redirection..\n");
		}
	}


	// register (shutdown)
	wprintf(L"******** RegisterShutdownScript [%s]\n", REG_PATH_SHUTDOWN);
	RegisterShutdownScript(HKEY_LOCAL_MACHINE, REG_PATH_SHUTDOWN);

	// register (logoff)
	wprintf(L"======== RegisterLogoffScript [%s]\n", REG_PATH_LOGOFF);
	RegisterLogoffScript(HKEY_CURRENT_USER, REG_PATH_LOGOFF);

	// register (state)
	// - Machine
	WCHAR machine[MAX_PATH] = {0, };
	wcscpy_s(machine, MAX_PATH, REG_PATH_STATE);
	wcscat_s(machine, MAX_PATH, L"\\Machine\\Scripts\\Shutdown");
	wprintf(L"******** RegisterShutdownScript [%s]\n", machine);
	RegisterShutdownScript(HKEY_LOCAL_MACHINE, machine);

	// - S-1-5-21-xxxx..
	WCHAR statelogoff[MAX_PATH] = {0, };
	FindStateLogoffPath(statelogoff, MAX_PATH);
	wprintf(L"======== RegisterLogoffScript [%s]\n", statelogoff);
	RegisterLogoffScript(HKEY_LOCAL_MACHINE, statelogoff);

	return TRUE;
}


BOOL UninstallScript()
{
	// delete scripts
	WCHAR shutdownpath[MAX_PATH] = {0, };
	GetGroupPolicyPath(shutdownpath, MAX_PATH);
	wcscat_s(shutdownpath, MAX_PATH, L"\\Machine\\Scripts\\Shutdown\\safehardtray.bat");

	WCHAR logoffpath[MAX_PATH] = {0, };
	GetGroupPolicyPath(logoffpath, MAX_PATH);
	wcscat_s(logoffpath, MAX_PATH, L"\\User\\Scripts\\Logoff\\safehardtray.bat");

	HANDLE hFile = INVALID_HANDLE_VALUE;
    PVOID OldValue = NULL;

    if (Wow64DisableWow64FsRedirection(&OldValue)) {

		printf("disable redirection..\n");

		if (DeleteFileW(shutdownpath) == TRUE) {
			wprintf(L"delete shutdown script file..[%s]", shutdownpath);
		} else {
			wprintf(L"delete shutdown script fail !!![%s]\n", shutdownpath);
		}
		if (DeleteFileW(logoffpath) == TRUE) {
			wprintf(L"delete logoff script file..[%s]\n", logoffpath);
		} else {
			wprintf(L"delete logoff script fail !!![%s]\n", logoffpath);
		}

		if (FALSE == Wow64RevertWow64FsRedirection(OldValue)) {
			printf("re-enable redirection fail !!!\n");
		} else {
			printf("re-enable redirection..\n");
		}
	}


	// delete register
	// - Shutdown (SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Shutdown)
	BOOL bSuccess = RegDeleteNode(HKEY_LOCAL_MACHINE, REG_PATH_SHUTDOWN);
	if (bSuccess == TRUE) {
		printf("delete reg (shutdown) success..\n");
	} else {
		printf("delete reg (shutdown) fail !!!\n");
	}

	// - Logoff (HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logoff)
	bSuccess = RegDeleteNode(HKEY_CURRENT_USER, REG_PATH_LOGOFF);
	if (bSuccess == TRUE) {
		printf("delete reg (logoff) success..\n");
	} else {
		printf("delete reg (logoff) fail !!!\n");
	}
	
	// - State
	//   - Shutdown (HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\State\Machine\Scripts\Shutdown)
	bSuccess = RegDeleteNode(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\Machine\\Scripts\\Shutdown");
	if (bSuccess == TRUE) {
		printf("delete reg (logoff) success..\n");
	} else {
		printf("delete reg (logoff) fail !!!\n");
	}

	//   - Logoff (HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\State\S-1-5-21-532101069-4145159380-969375416-1001\Scripts\Logoff)
	// find path
	WCHAR statelogoff[MAX_PATH] = {0, }; 
	FindStateLogoffPath(statelogoff, MAX_PATH);
	bSuccess = RegDeleteNode(HKEY_LOCAL_MACHINE, statelogoff);
	if (bSuccess == TRUE) {
		printf("delete reg (logoff) success..\n");
	} else {
		printf("delete reg (logoff) fail !!!\n");
	}

	return TRUE;
}


BOOL GetGroupPolicyPath(WCHAR path[], int size)
{
	wmemset(path, 0, size); 

	GetEnvironmentVariableW(L"systemroot", path, MAX_PATH);

	wcscat_s(path, size, L"\\system32\\GroupPolicy");

	return TRUE;
}


BOOL GetSafeHardTrayPath(WCHAR* path, int size)
{
	wmemset(path, 0, size);

	HRESULT hr = SHGetSpecialFolderPathW(NULL, path, CSIDL_PROGRAM_FILES, TRUE);
	if (!SUCCEEDED(hr)) {
		printf("fail: get program folder...\n");
		return FALSE;
	} else {
		wprintf(L"program files folder: [%s]\n", path);
		wcscat_s(path, MAX_PATH, L"\\wizvera\\safehard\\safehardtray.exe");
		wprintf(L"module path: [%s]\n", path);
	}
	
	wprintf(L"GetProgramPath:: get complete..[%s]\n", path);

	return TRUE;
}


BOOL FindStateLogoffPath(WCHAR* path, int size)
{
	int i, j;
	WCHAR keyname[MAX_PATH] = {0, }; 
	State_Subs stSubkeys;
	State_Subs stSubkeys2;

	wmemset(path, 0, size);
	RegGetKeyNames(&stSubkeys, HKEY_LOCAL_MACHINE, REG_PATH_STATE);
	
	for (i = 0; i < SUBCOUNT; i++) {
		wprintf(L"search key: [%s]\n", stSubkeys.subs[i]);

		if (wcscmp(stSubkeys.subs[i], L"Machine") == 0) {
			continue;
		}
		wcscpy_s(keyname, MAX_PATH, REG_PATH_STATE); 
		wcscat_s(keyname, MAX_PATH, L"\\");
		wcscat_s(keyname, MAX_PATH, stSubkeys.subs[i]);

		RegGetKeyNames(&stSubkeys2, HKEY_LOCAL_MACHINE, keyname);

		for (j = 0; j < SUBCOUNT; j++) {
			wprintf(L"search subkeys2[j]: [%s]\n", stSubkeys2.subs[j]);
			if (wcscmp(stSubkeys2.subs[j], L"Scripts") == 0) {
				wcscpy_s(path, MAX_PATH, REG_PATH_STATE);
				wcscat_s(path, MAX_PATH, L"\\");
				wcscat_s(path, MAX_PATH, stSubkeys.subs[i]);
				wcscat_s(path, MAX_PATH, L"\\Scripts\\Logoff");
				printf("\n$$$$$$$$$$$$$ found State Scripts $$$$$$$$$$$$\n\n");
				wprintf(L"path: [%s]\n", path);
				break;
			}
		}
	}
	
	return TRUE;
}


BOOL RegCheckNode(HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	LONG lResult = 0;
	HKEY hKey;

	lResult = RegOpenKeyExW(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	if (lResult == ERROR_SUCCESS) {
		printf("RegCheckNode:: check key complete\n");
		return TRUE;
	} else {
		printf("RegCheckNode:: check key fail !!!\n");
		return FALSE;
	}
}


BOOL RegCreateNode(HKEY* phKey, HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	LONG lResult = 0;

	lResult = RegOpenKeyExW(hKeyRoot, lpSubKey, 0, KEY_READ, phKey);
	if (lResult == ERROR_SUCCESS) {
		printf("RegCreateNode:: open key complete\n");
		return TRUE;
	}

	lResult = RegCreateKeyExW(hKeyRoot,
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
	} else {
		printf("RegCreateNode:: create key fail !!!\n");
		return FALSE;
	}
}


BOOL RegSetValueString(HKEY* phKey, LPCWSTR lpValue, LPCWSTR lpData)
{
	LONG lResult = 0;

	lResult = RegSetValueExW(*phKey, 
						    lpValue, 
							0, 
							REG_SZ, 
							(const BYTE*)lpData, 
							wcslen(lpData) * sizeof(WCHAR) + 1);
	if (lResult == ERROR_SUCCESS) {
		wprintf(L"RegSetValueString:: set key(s) complete\n");
		return TRUE;
	} else {
		wprintf(L"RegSetValueString:: set key(s) failed !!!\n");
		return FALSE;
	}
}


BOOL RegSetValueDword(HKEY* phKey, LPCWSTR lpValue, DWORD dwData)
{
	LONG lResult = 0;

	lResult = RegSetValueExW(*phKey, 
						    lpValue, 
							0, 
							REG_DWORD, 
							(LPBYTE)&dwData, 
							sizeof(dwData));
	if (lResult == ERROR_SUCCESS) {
		printf("RegSetValueD:: set key(d) complete\n");
		return TRUE;
	} else {
		printf("RegSetValueD:: set key(d) failed !!!\n");
		return FALSE;
	}
}


BOOL RegDeleteNode(HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	WCHAR szDelKey[MAX_PATH * 2] = {0, };

	StringCchCopyW(szDelKey, MAX_PATH * 2, lpSubKey);
	return RegDeleteNodeRecursive(hKeyRoot, szDelKey);
}


BOOL RegDeleteNodeRecursive(HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	LPWSTR lpEnd;
	LONG lResult = 0;
	DWORD dwSize = 0;
	WCHAR szName[MAX_PATH] = {0, };
	HKEY hKey;
	FILETIME ftWrite;
	int loop_cnt = 0;

	printf("RegDeleteNodeRecursive:: start~~~\n");
	printf("lpSubKey: [%s]\n", lpSubKey);

	lResult = RegDeleteKeyW(hKeyRoot, lpSubKey);
    if (lResult == ERROR_SUCCESS) {
		printf("RegDeleteNodeRecursive:: delete key success~~~\n");
		return TRUE;
	} else {
		printf("RegDeleteNodeRecursive:: delete key fail~~~\n");
	}

	lResult = RegOpenKeyExW(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS) {
        if (lResult == ERROR_FILE_NOT_FOUND) {
            printf("RegDeleteNodeRecursive:: key not found !!!\n");
            return TRUE;
        } 
        else {
            printf("RegDeleteNodeRecursive:: cannot opening key.\n");
            return FALSE;
        }
    }
	printf("RegDeleteNodeRecursive:: open key~~~\n");

	// Check for an ending slash and add one if it is missing.
    lpEnd = (LPWSTR)(lpSubKey + lstrlenW(lpSubKey));
    if (*(lpEnd - 1) != L'\\') 
    {
        *lpEnd = L'\\';
        lpEnd++;
        *lpEnd = L'\0';
    }

    // Enumerate the keys
    dwSize = MAX_PATH;
    lResult = RegEnumKeyExW(hKey, 0, szName, &dwSize, NULL,
                           NULL, NULL, &ftWrite);
	loop_cnt = 0;
    if (lResult == ERROR_SUCCESS) {
        do {
			printf("RegDeleteNodeRecursive:: enum key~~~\n");
			loop_cnt++;

            StringCchCopyW(lpEnd, MAX_PATH * 2, szName);
            if (!RegDeleteNodeRecursive(hKeyRoot, lpSubKey)) {
				printf("RegDeleteNodeRecursive:: do while break~~~\n");
                break;
            }
            dwSize = MAX_PATH;
            lResult = RegEnumKeyExW(hKey, 0, szName, &dwSize, NULL,
                                   NULL, NULL, &ftWrite);
        } while (lResult == ERROR_SUCCESS);
    } else {
		printf("RegDeleteNodeRecursive:: enum key fail !!!\n");
	}

    lpEnd--;
    *lpEnd = L'\0';

    RegCloseKey(hKey);

    // Try again to delete the key.
    lResult = RegDeleteKeyW(hKeyRoot, lpSubKey);
    if (lResult == ERROR_SUCCESS) {
		printf("RegDeleteNodeRecursive:: delete key complete.\n");
        return TRUE;
	} else {
		printf("RegDeleteNodeRecursive:: delete key failed !!!\n");
		return FALSE;
	}
}


DWORD RegGetSubKeyCount(HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	HKEY key;
	DWORD subkeys = 0;
	LONG lResult = RegOpenKeyExW(hKeyRoot, lpSubKey, 0, KEY_READ, &key);

	if (lResult != ERROR_SUCCESS) {
		printf("RegGetKeyName:: open key fail !!!\n");
		return FALSE;
	}
	
	// info
	lResult = RegQueryInfoKey(key, NULL, NULL, NULL, &subkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	if (lResult == ERROR_SUCCESS) {
		printf("RegGetKeyName:: get key info. success. [%d]\n", (int)subkeys);
	} else {
		printf("RegGetKeyName:: get key info. fail !!!\n");
		return FALSE;
	}

	RegCloseKey(key);

	return subkeys;
}


BOOL RegGetKeyNames(State_Subs* subkeynames, HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	DWORD i;
	HKEY key;
	DWORD subkeys = 0;
	LONG lResult = 0;
	WCHAR name[MAX_PATH] = {0, }; 
	DWORD namesize = MAX_PATH;
	
	subkeys = RegGetSubKeyCount(hKeyRoot, lpSubKey);
	printf("RegGetKeyName:: count subkey [%d]\n", subkeys);

	for (i = 0; i < SUBCOUNT; i++) {
		wmemset(subkeynames->subs[i], 0, SUBSIZE);
	}

	lResult = RegOpenKeyExW(hKeyRoot, lpSubKey, 0, KEY_READ, &key);
	if (lResult != ERROR_SUCCESS) {
		printf("RegGetKeyName:: open key fail !!!\n");
		return FALSE;
	} else {
		printf("RegGetKeyName:: open key ok\n");
	}

	for (i = 0; i < subkeys; i++) {
		printf("RegGetKeyName:: loop subkey [%d]\n", i);
		namesize = MAX_PATH;
		lResult = RegEnumKeyExW(key, i, name, &namesize, NULL, NULL, NULL, NULL);
		if (lResult == ERROR_SUCCESS) {
			wprintf(L"RegGetKeyName:: get subkey name success. [%s]\n", name);

			wcscpy_s(subkeynames->subs[i], SUBSIZE, name);
		} else {
			printf("RegGetKeyName:: get subkey name fail !!!\n");
		}
	}

	RegCloseKey(key);

	return TRUE;
}


BOOL RegisterShutdownScript(HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	// create
	HKEY hKey;
	WCHAR szSubKey[MAX_PATH] = {0, };
	WCHAR machinepath[MAX_PATH] = {0, };

	wcscpy_s(szSubKey, MAX_PATH, lpSubKey);

	BOOL bSuccess = RegCreateNode(&hKey, hKeyRoot, szSubKey);
	if (bSuccess != TRUE) {
		printf("RegisterShutdownScript:: create fail !!!\n");
		return FALSE;
	} else {
		printf("RegisterShutdownScript:: create ok\n");
	}

	// check
	wcscat_s(szSubKey, MAX_PATH, L"\\0");
	bSuccess = RegCheckNode(hKeyRoot, szSubKey);
	if (bSuccess != TRUE) {
		bSuccess = RegCreateNode(&hKey, hKeyRoot, szSubKey);
		if (bSuccess != TRUE) {
			printf("RegisterShutdownScript:: create 2 fail !!!\n");
			return FALSE;
		} else {
			printf("RegisterShutdownScript:: create 2 ok\n");
		}
	}

	GetGroupPolicyPath(machinepath, MAX_PATH);
	wcscat_s(machinepath, MAX_PATH, L"\\Machine");

	// set value
	bSuccess = RegSetValueString(&hKey, L"DisplayName", L"Local Group Policy");
	bSuccess = RegSetValueString(&hKey, L"FileSysPath", machinepath);
	bSuccess = RegSetValueString(&hKey, L"GPO-ID", L"LocalGPO");
	bSuccess = RegSetValueString(&hKey, L"GPOName", L"Local Group Policy");
	bSuccess = RegSetValueDword(&hKey, L"PSScriptOrder", 1);
	bSuccess = RegSetValueString(&hKey, L"SOM-ID", L"Local");

	RegCloseKey(hKey);

	// sub
	wcscat_s(szSubKey, MAX_PATH, L"\\0");
	BOOL bSuccess4 = RegCreateNode(&hKey, hKeyRoot, szSubKey);

	// set value
	bSuccess = RegSetValueDword(&hKey, L"ExecTime", 0);
	bSuccess = RegSetValueDword(&hKey, L"IsPowershell", 0);
	bSuccess = RegSetValueString(&hKey, L"Parameters", L"");
	bSuccess = RegSetValueString(&hKey, L"Script", L"safehardtray.bat");

	// close
	RegCloseKey(hKey);

	return TRUE;
}


BOOL RegisterLogoffScript(HKEY hKeyRoot, LPCWSTR lpSubKey)
{
	// create
	HKEY hKey;
	BOOL bSuccess;
	WCHAR szSubKey[MAX_PATH] = {0, };
	WCHAR userpath[MAX_PATH] = {0, };

	wcscpy_s(szSubKey, MAX_PATH, lpSubKey);
	bSuccess = RegCreateNode(&hKey, hKeyRoot, szSubKey);
	if (bSuccess != TRUE) {
		printf("RegisterLogoffScript:: create fail !!!\n");
		return FALSE;
	} else {
		printf("RegisterLogoffScript:: create ok\n");
	}

	// check
	wcscat_s(szSubKey, MAX_PATH, L"\\0");
	bSuccess = RegCheckNode(hKeyRoot, szSubKey);
	if (bSuccess != TRUE) {
		bSuccess = RegCreateNode(&hKey, hKeyRoot, szSubKey);
		if (bSuccess != TRUE) {
			printf("RegisterLogoffScript:: create 2 fail !!!\n");
			return FALSE;
		} else {
			printf("RegisterLogoffScript:: create 2 fail !!!\n");
		}
	}

	GetGroupPolicyPath(userpath, MAX_PATH);
	wcscat_s(userpath, MAX_PATH, L"\\User");

	printf("RegisterLogoffScript:: set ready...\n\n");

	// set value
	bSuccess = RegSetValueString(&hKey, L"DisplayName", L"Local Group Policy");
	bSuccess = RegSetValueString(&hKey, L"FileSysPath", userpath);
	bSuccess = RegSetValueString(&hKey, L"GPO-ID", L"LocalGPO");
	bSuccess = RegSetValueString(&hKey, L"GPOName", L"Local Group Policy");
	bSuccess = RegSetValueDword(&hKey, L"PSScriptOrder", 1);
	bSuccess = RegSetValueString(&hKey, L"SOM-ID", L"Local");

	RegCloseKey(hKey);

	// sub
	wcscat_s(szSubKey, MAX_PATH, L"\\0");
	bSuccess = RegCreateNode(&hKey, hKeyRoot, szSubKey);
	if (bSuccess != TRUE) {
		return FALSE;
	}

	// set value
	bSuccess = RegSetValueDword(&hKey, L"ExecTime", 0);
	bSuccess = RegSetValueDword(&hKey, L"IsPowershell", 0);
	bSuccess = RegSetValueString(&hKey, L"Parameters", L"");
	bSuccess = RegSetValueString(&hKey, L"Script", L"safehardtray.bat");

	// close
	RegCloseKey(hKey);

	return TRUE;
}


BOOL WriteShutdownScript(LPCWSTR lpFilePath)
{
	WCHAR szPath[MAX_PATH] = {0, };

	FILE* pFile = _wfopen(lpFilePath, L"w");
	if (pFile == NULL) {
		wprintf(L"file open fail !!! [%s]\n", lpFilePath);
		return FALSE;
	} else {
		wprintf(L"file created.. [%s]\n", lpFilePath);
	}

	GetSafeHardTrayPath(szPath, MAX_PATH);

	fwprintf(pFile, L"\"%s\" close\n", szPath);
	fwprintf(pFile, L"Timeout /t 4 >nul\n");
	fwprintf(pFile, L"\n");
	fwprintf(pFile, L"mkdir %%TEMP%%\\SHUTDOWN5\n");
	fwprintf(pFile, L"\n");

	fclose(pFile);

	printf("shutdown file write complete..\n");

	return TRUE;
}


BOOL WriteLogoffScript(LPCWSTR lpFilePath)
{
	WCHAR szPath[MAX_PATH] = {0, };

	FILE* pFile = _wfopen(lpFilePath, L"w");
	if (pFile == NULL) {
		wprintf(L"file open fail !!! [%s]\n", lpFilePath);
		return FALSE;
	} else {
		wprintf(L"file created.. [%s]\n", lpFilePath);
	}

	GetSafeHardTrayPath(szPath, MAX_PATH);

	fwprintf(pFile, L"\"%s\" close\n", szPath);
	fwprintf(pFile, L"Timeout /t 4 >nul\n");
	fwprintf(pFile, L"\n");
	fwprintf(pFile, L"mkdir %%TEMP%%\\LOGOFF5\n");
	fwprintf(pFile, L"\n");

	fclose(pFile);

	printf("logoff file write complete..\n");

	return TRUE;
}

