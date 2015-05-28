#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <vector>
using namespace std;

#pragma comment(lib, "Advapi32.lib")

BOOL InstallScript();
BOOL UninstallScript();

string GetGroupPolicyPath();
string GetProgramPath();
string FindStateLogoffPath();

BOOL RegDeleteNode(HKEY hKeyRoot, LPCSTR lpSubKey);
BOOL RegDeleteNodeRecursive(HKEY hKeyRoot, LPCSTR lpSubKey);

BOOL RegCreateNode(HKEY* phKey, HKEY hKeyRoot, LPCSTR lpSubKey);
BOOL RegSetValueString(HKEY* phKey, LPCSTR lpValue, LPCSTR lpData);
BOOL RegSetValueD(HKEY* phKey, LPCSTR lpValue, DWORD dwData);

BOOL RegCheckNode(HKEY hKeyRoot, LPCSTR lpSubKey);

DWORD RegGetSubKeyCount(HKEY hKeyRoot, LPCSTR lpSubKey);
BOOL RegGetKeyNames(vector<string>& subkeynames, HKEY hKeyRoot, LPCSTR lpSubKey);

BOOL RegisterShutdownScript(HKEY hKeyRoot, LPCSTR lpSubKey);
BOOL RegisterLogoffScript(HKEY hKeyRoot, LPCSTR lpSubKey);

BOOL WriteShutdownScript(const string& path);
BOOL WriteLogoffScript(const string& path);

#define REG_PATH_SHUTDOWN	"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown"
#define REG_PATH_LOGOFF		"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff"
#define REG_PATH_STATE		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State"


int main()
{

	//InstallScript();
	
	UninstallScript();

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
	string shutdownpath = GetGroupPolicyPath();
	shutdownpath += "\\Machine\\Scripts\\Shutdown\\safehardtray.bat";

	string logoffpath = GetGroupPolicyPath();
	logoffpath += "\\User\\Scripts\\Logoff\\safehardtray.bat";

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
	string subkey = REG_PATH_SHUTDOWN;
	printf("******** RegisterShutdownScript [%s]\n", subkey.c_str());
	RegisterShutdownScript(HKEY_LOCAL_MACHINE, subkey.c_str());

	// register (logoff)
	subkey = REG_PATH_LOGOFF;
	printf("======== RegisterLogoffScript [%s]\n", subkey.c_str());
	RegisterLogoffScript(HKEY_CURRENT_USER, subkey.c_str());

	// register (state)
	// - Machine
	string machine = REG_PATH_STATE;
	machine += "\\Machine\\Scripts\\Shutdown";
	printf("******** RegisterShutdownScript [%s]\n", machine.c_str());
	RegisterShutdownScript(HKEY_LOCAL_MACHINE, machine.c_str());

	// - S-1-5-21-xxxx..
	string statelogoff = FindStateLogoffPath();
	printf("======== RegisterLogoffScript [%s]\n", statelogoff.c_str());
	RegisterLogoffScript(HKEY_LOCAL_MACHINE, statelogoff.c_str());

	return TRUE;
}


BOOL UninstallScript()
{
	// delete scripts
	string shutdownpath = GetGroupPolicyPath();
	shutdownpath += "\\Machine\\Scripts\\Shutdown\\safehardtray.bat";

	string logoffpath = GetGroupPolicyPath();
	logoffpath += "\\User\\Scripts\\Logoff\\safehardtray.bat";

	HANDLE hFile = INVALID_HANDLE_VALUE;
    PVOID OldValue = NULL;

    if (Wow64DisableWow64FsRedirection(&OldValue)) {

		printf("disable redirection..\n");

		if (DeleteFile(shutdownpath.c_str()) == TRUE) {
			printf("delete shutdown script file..\[%s]", shutdownpath.c_str());
		} else {
			printf("delete shutdown script fail !!![%s]\n", shutdownpath.c_str());
		}
		if (DeleteFile(logoffpath.c_str()) == TRUE) {
			printf("delete logoff script file..[%s]\n", logoffpath.c_str());
		} else {
			printf("delete logoff script fail !!![%s]\n", logoffpath.c_str());
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
	bSuccess = RegDeleteNode(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\Machine\\Scripts\\Shutdown");
	if (bSuccess == TRUE) {
		printf("delete reg (logoff) success..\n");
	} else {
		printf("delete reg (logoff) fail !!!\n");
	}
	//   - Logoff (HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\State\S-1-5-21-532101069-4145159380-969375416-1001\Scripts\Logoff)
	// find path
	bSuccess = RegDeleteNode(HKEY_LOCAL_MACHINE, );
	if (bSuccess == TRUE) {
		printf("delete reg (logoff) success..\n");
	} else {
		printf("delete reg (logoff) fail !!!\n");
	}


	return TRUE;
}


string GetGroupPolicyPath()
{
	char szGPPath[MAX_PATH] = {0, };

	GetEnvironmentVariableA("systemroot", szGPPath, MAX_PATH);

	string strGPPath = szGPPath;
	strGPPath += "\\system32\\GroupPolicy";

	return strGPPath;
}


string GetProgramPath()
{
	char szModulePath[MAX_PATH] = {0, };
    string strModPath = "";
    HMODULE hMod = ::LoadLibraryA("SafehardHSM.dll");
    if (hMod == NULL) {
		printf("LoadLibrary fail !!!\n");
        return strModPath;
	}

    ::GetModuleFileNameA(hMod, szModulePath, sizeof(szModulePath));

	strModPath = szModulePath;

	printf("GetProgramPath:: get complete..[%s]\n", strModPath.c_str());

	::FreeLibrary(hMod);

	return strModPath;
}


string FindStateLogoffPath()
{
	string strPath = "";
	vector<string> subkeys;
	vector<string> subkeys2;
	string keyname = REG_PATH_STATE;

	RegGetKeyNames(subkeys, HKEY_LOCAL_MACHINE, keyname.c_str());
	printf(".result: [%d][]\n", subkeys.size());
	
	for (int i = 0; i < subkeys.size(); i++) {
		printf("search key: [%s]\n", subkeys.at(i).c_str());

		if (subkeys.at(i).compare("Machine") == 0) {
			continue;
		}
		keyname = REG_PATH_STATE; 
		keyname += "\\";
		keyname += subkeys.at(i);

		RegGetKeyNames(subkeys2, HKEY_LOCAL_MACHINE, keyname.c_str());
		printf("2 result: [%d][%d]\n", i, subkeys2.size());
		printf("\n");

		for (int j = 0; j < subkeys2.size(); j++) {
			printf("search subkeys2[j]: [%s]\n", subkeys2.at(j).c_str());
			if (subkeys2.at(j).compare("Scripts") == 0) {
				strPath = REG_PATH_STATE;
				strPath += "\\";
				strPath += subkeys.at(i);
				strPath += "\\Scripts\\Logoff";
				printf("\n$$$$$$$$$$$$$ found State Scripts $$$$$$$$$$$$\n\n");
				printf("path: [%s]\n", strPath.c_str());
				break;
			}
		}
	}
	
	return strPath;
}


BOOL RegCheckNode(HKEY hKeyRoot, LPCSTR lpSubKey)
{
	LONG lResult = 0;
	HKEY hKey;

	lResult = RegOpenKeyExA(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	if (lResult == ERROR_SUCCESS) {
		printf("RegCheckNode:: check key complete\n");
		return TRUE;
	} else {
		printf("RegCheckNode:: check key fail !!!\n");
		return FALSE;
	}
}


BOOL RegCreateNode(HKEY* phKey, HKEY hKeyRoot, LPCSTR lpSubKey)
{
	LONG lResult = 0;

	lResult = RegOpenKeyExA(hKeyRoot, lpSubKey, 0, KEY_READ, phKey);
	if (lResult == ERROR_SUCCESS) {
		printf("RegCreateNode:: open key complete\n");
		return TRUE;
	}

	lResult = RegCreateKeyExA(hKeyRoot,
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


BOOL RegSetValueString(HKEY* phKey, LPCSTR lpValue, LPCSTR lpData)
{
	LONG lResult = 0;

	lResult = RegSetValueExA(*phKey, 
						    lpValue, 
							0, 
							REG_SZ, 
							(LPBYTE)lpData, 
							lstrlen(lpData) + 1);
	if (lResult == ERROR_SUCCESS) {
		printf("RegSetValueString:: set key(s) complete\n");
		return TRUE;
	} else {
		printf("RegSetValueString:: set key(s) failed !!!\n");
		return FALSE;
	}
}


BOOL RegSetValueD(HKEY* phKey, LPCSTR lpValue, DWORD dwData)
{
	LONG lResult = 0;

	lResult = RegSetValueExA(*phKey, 
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


BOOL RegDeleteNode(HKEY hKeyRoot, LPCSTR lpSubKey)
{
	char szDelKey[MAX_PATH * 2] = {0, };

	StringCchCopyA(szDelKey, MAX_PATH * 2, lpSubKey);
	return RegDeleteNodeRecursive(hKeyRoot, szDelKey);
}


BOOL RegDeleteNodeRecursive(HKEY hKeyRoot, LPCSTR lpSubKey)
{
	LPSTR lpEnd;
	LONG lResult = 0;
	DWORD dwSize = 0;
	char szName[MAX_PATH] = {0, };
	HKEY hKey;
	FILETIME ftWrite;

	printf("RegDeleteNodeRecursive:: start~~~\n");
	printf("lpSubKey: [%s]\n", lpSubKey);

	lResult = RegDeleteKeyA(hKeyRoot, lpSubKey);
    if (lResult == ERROR_SUCCESS) {
		printf("RegDeleteNodeRecursive:: delete key success~~~\n");
		return TRUE;
	} else {
		printf("RegDeleteNodeRecursive:: delete key fail~~~\n");
	}

	lResult = RegOpenKeyExA(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
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
    lpEnd = const_cast<LPSTR>(lpSubKey + lstrlenA(lpSubKey));
    if (*(lpEnd - 1) != '\\') 
    {
        *lpEnd =  '\\';
        lpEnd++;
        *lpEnd =  '\0';
    }

    // Enumerate the keys
    dwSize = MAX_PATH;
    lResult = RegEnumKeyExA(hKey, 0, szName, &dwSize, NULL,
                           NULL, NULL, &ftWrite);
	int loop_cnt = 0;

    if (lResult == ERROR_SUCCESS) {
        do {
			printf("RegDeleteNodeRecursive:: enum key~~~\n");
			loop_cnt++;

            StringCchCopyA(lpEnd, MAX_PATH*2, szName);
            if (!RegDeleteNodeRecursive(hKeyRoot, lpSubKey)) {
				printf("RegDeleteNodeRecursive:: do while break~~~\n");
                break;
            }
            dwSize = MAX_PATH;
            lResult = RegEnumKeyExA(hKey, 0, szName, &dwSize, NULL,
                                   NULL, NULL, &ftWrite);
        } while (lResult == ERROR_SUCCESS);
    } else {
		printf("RegDeleteNodeRecursive:: enum key fail !!!\n");
	}

    lpEnd--;
    *lpEnd = '\0';

    RegCloseKey(hKey);

    // Try again to delete the key.
    lResult = RegDeleteKeyA(hKeyRoot, lpSubKey);
    if (lResult == ERROR_SUCCESS) {
		printf("RegDeleteNodeRecursive:: delete key complete.\n");
        return TRUE;
	} else {
		printf("RegDeleteNodeRecursive:: delete key failed !!!\n");
		return FALSE;
	}
}


DWORD RegGetSubKeyCount(HKEY hKeyRoot, LPCSTR lpSubKey)
{
	HKEY key;
	LONG lResult = RegOpenKeyExA(hKeyRoot, lpSubKey, 0, KEY_READ, &key);
	if (lResult != ERROR_SUCCESS) {
		printf("RegGetKeyName:: open key fail !!!\n");
		return FALSE;
	}
	
	// info
	DWORD subkeys = 0;
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


BOOL RegGetKeyNames(vector<string>& subkeynames, HKEY hKeyRoot, LPCSTR lpSubKey)
{
	DWORD subkeys = RegGetSubKeyCount(hKeyRoot, lpSubKey);
		
	printf("RegGetKeyName:: count subkey [%d]\n", subkeys);

	vector<string>().swap(subkeynames);

	HKEY key;
	LONG lResult = RegOpenKeyExA(hKeyRoot, lpSubKey, 0, KEY_READ, &key);
	if (lResult != ERROR_SUCCESS) {
		printf("RegGetKeyName:: open key fail !!!\n");
		return FALSE;
	} else {
		printf("RegGetKeyName:: open key ok\n");
	}

	for (DWORD i = 0; i < subkeys; i++) {
		printf("RegGetKeyName:: loop subkey [%d]\n", i);

		char name[MAX_PATH] = {0, }; 
		DWORD namesize = MAX_PATH;
		lResult = RegEnumKeyEx(key, i, name, &namesize, NULL, NULL, NULL, NULL);
		if (lResult == ERROR_SUCCESS) {
			printf("RegGetKeyName:: get subkey name success. [%s]\n", name);
			subkeynames.push_back(name);
		} else {
			printf("RegGetKeyName:: get subkey name fail !!!\n");
		}
	}

	RegCloseKey(key);
}


BOOL RegisterShutdownScript(HKEY hKeyRoot, LPCSTR lpSubKey)
{
	// create
	string strSubKey = lpSubKey;
	HKEY hKey;
	BOOL bSuccess = RegCreateNode(&hKey, hKeyRoot, strSubKey.c_str());
	if (bSuccess != TRUE) {
		printf("RegisterShutdownScript:: create fail !!!\n");
		return FALSE;
	} else {
		printf("RegisterShutdownScript:: create ok\n");
	}

	// check
	strSubKey += "\\0";
	bSuccess = RegCheckNode(hKeyRoot, strSubKey.c_str());
	if (bSuccess != TRUE) {
		bSuccess = RegCreateNode(&hKey, hKeyRoot, strSubKey.c_str());
		if (bSuccess != TRUE) {
			printf("RegisterShutdownScript:: create 2 fail !!!\n");
			return FALSE;
		} else {
			printf("RegisterShutdownScript:: create 2 ok\n");
		}
	}

	string machinepath = GetGroupPolicyPath();
	machinepath += "\\Machine";

	// set value
	bSuccess = RegSetValueString(&hKey, "DisplayName", "Local Group Policy");
	bSuccess = RegSetValueString(&hKey, "FileSysPath", machinepath.c_str());
	bSuccess = RegSetValueString(&hKey, "GPO-ID", "LocalGPO");
	bSuccess = RegSetValueString(&hKey, "GPOName", "Local Group Policy");
	bSuccess = RegSetValueD(&hKey, "PSScriptOrder", 1);
	bSuccess = RegSetValueString(&hKey, "SOM-ID", "Local");

	RegCloseKey(hKey);

	// sub
	strSubKey += "\\0";
	BOOL bSuccess4 = RegCreateNode(&hKey, hKeyRoot, strSubKey.c_str());

	// set value
	bSuccess = RegSetValueD(&hKey, "ExecTime", 0);
	bSuccess = RegSetValueD(&hKey, "IsPowershell", 0);
	bSuccess = RegSetValueString(&hKey, "Parameters", "");
	bSuccess = RegSetValueString(&hKey, "Script", "safehardtray.bat");

	// close
	RegCloseKey(hKey);

	return TRUE;
}


BOOL RegisterLogoffScript(HKEY hKeyRoot, LPCSTR lpSubKey)
{
	// create
	string strSubKey = lpSubKey;
	HKEY hKey;
	BOOL bSuccess = RegCreateNode(&hKey, hKeyRoot, strSubKey.c_str());
	if (bSuccess != TRUE) {
		printf("RegisterLogoffScript:: create fail !!!\n");
		return FALSE;
	} else {
		printf("RegisterLogoffScript:: create ok\n");
	}

	// check
	strSubKey += "\\0";
	bSuccess = RegCheckNode(hKeyRoot, strSubKey.c_str());
	if (bSuccess != TRUE) {
		bSuccess = RegCreateNode(&hKey, hKeyRoot, strSubKey.c_str());
		if (bSuccess != TRUE) {
			printf("RegisterLogoffScript:: create 2 fail !!!\n");
			return FALSE;
		} else {
			printf("RegisterLogoffScript:: create 2 fail !!!\n");
		}
	}

	string userpath = GetGroupPolicyPath();
	userpath += "\\User";

	printf("RegisterLogoffScript:: set ready...\n\n");

	// set value
	bSuccess = RegSetValueString(&hKey, "DisplayName", "Local Group Policy");
	bSuccess = RegSetValueString(&hKey, "FileSysPath", userpath.c_str());
	bSuccess = RegSetValueString(&hKey, "GPO-ID", "LocalGPO");
	bSuccess = RegSetValueString(&hKey, "GPOName", "Local Group Policy");
	bSuccess = RegSetValueD(&hKey, "PSScriptOrder", 1);
	bSuccess = RegSetValueString(&hKey, "SOM-ID", "Local");

	RegCloseKey(hKey);

	// sub
	strSubKey += "\\0";
	bSuccess = RegCreateNode(&hKey, hKeyRoot, strSubKey.c_str());
	if (bSuccess != TRUE) {
		return FALSE;
	}

	// set value
	bSuccess = RegSetValueD(&hKey, "ExecTime", 0);
	bSuccess = RegSetValueD(&hKey, "IsPowershell", 0);
	bSuccess = RegSetValueString(&hKey, "Parameters", "");
	bSuccess = RegSetValueString(&hKey, "Script", "safehardtray.bat");

	// close
	RegCloseKey(hKey);

	return TRUE;
}


BOOL WriteShutdownScript(const string& path)
{
	FILE* pFile = fopen(path.c_str(), "w");
	if (pFile == NULL) {
		printf("file open fail !!! [%s]\n", path.c_str());
		return FALSE;
	} else {
		printf("file created.. [%s]\n", path.c_str());
	}

	fprintf(pFile, "%%TEMP%%\\safehardtray.exe close\n");
	fprintf(pFile, "Timeout /t 4 >nul\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "mkdir D:\\SHUTDOWN5\n");
	fprintf(pFile, "\n");

	fclose(pFile);

	printf("shutdown file write complete..\n");

	return TRUE;
}


BOOL WriteLogoffScript(const string& path)
{
	FILE* pFile = fopen(path.c_str(), "w");
	if (pFile == NULL) {
		printf("file open fail !!! [%s]\n", path.c_str());
		return FALSE;
	} else {
		printf("file created.. [%s]\n", path.c_str());
	}

	fprintf(pFile, "%%TEMP%%\\safehardtray.exe close\n");
	fprintf(pFile, "Timeout /t 4 >nul\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "mkdir D:\\LOGOFF5\n");
	fprintf(pFile, "\n");

	fclose(pFile);

	printf("logoff file write complete..\n");

	return TRUE;
}

