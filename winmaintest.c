#include <windows.h>
#include <Shellapi.h>
#include <Shlobj.h>
#include <stdio.h>
//#include <VersionHelpers.h>
#include <wchar.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")

#define SAFEHARD_PATH	"\\wizvera\\safehard"
#define MODULE_NAME		"\\safehardmanager.exe"
#define CONF_PATH		"\\SafeHard\\conf"

const CLSID CLSID_localLow = {0xA520A1A4, 0x1780, 0x4FF6, 0xBD, 0x18, 0x16, 0x73, 0x43, 0xC5, 0xAF, 0x16};

int SetTrayConfig(const char* path, const char* value);


int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR szCmdLine,
				   int iCmdShow)
{
	int i = 0, ret = 0;
	int length = 0;
	char* szPathLocalLow = NULL;
	char* szTemp = NULL;
	char* szWin = NULL;
	char* pch = NULL;
	wchar_t* wszPath = NULL;
	char buff[10] = {0, };
	char szArg[MAX_PATH] = {0, };
	char szArgs[10][32] = {0, };
	char szPath[MAX_PATH] = {0, };
	char szPathTemp[MAX_PATH] = {0, };
	SHELLEXECUTEINFOA execinfo;

	HRESULT hr = SHGetSpecialFolderPathA(NULL, szPath, CSIDL_COMMON_DOCUMENTS, TRUE);
	if (!SUCCEEDED(hr)) {
		return 1;
	}
	strcat_s(szPath, MAX_PATH, SAFEHARD_PATH);
	strcat_s(szPath, MAX_PATH, MODULE_NAME);
	printf("path: %s\n", szPath);


	//MessageBox(NULL, GetCommandLine(), GetCommandLine(), MB_OK);

	strcpy_s(szArg, MAX_PATH, szCmdLine);

	pch = strtok(szArg, " ,.-");
	while (pch != NULL) {
		sprintf(szArgs[++i], "%s", pch);
		pch = strtok(NULL, " ,.-");
	}
	++i;

	MessageBox(NULL, itoa(i, buff, 10), "cmd line", MB_OK);
	MessageBox(NULL, szArgs[0], "cmd line", MB_OK);
	MessageBox(NULL, szArgs[1], "cmd line", MB_OK);


	//MessageBoxA(NULL, szPath, "test", MB_OK);
	//if (!IsWindowsVistaOrGreater()) {
		//MessageBox(NULL, "You need at least Windows Vista", "Version Not Supported", MB_OK);
	//} else {
		//MessageBox(NULL, "Vista or Later", "Windows Version", MB_OK);
	//}

	ZeroMemory( &execinfo, sizeof(execinfo) );
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = "open";
	execinfo.lpFile = szPath;
	execinfo.lpParameters = "white";
	execinfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;

	//ret = (int)ShellExecuteExA(&execinfo);
	//WaitForSingleObject(execinfo.hProcess, INFINITE);

	// LocalLow
	SHGetKnownFolderPath(&CLSID_localLow, 0, NULL, &wszPath);
	//MessageBoxW(NULL, wszPath, L"test", MB_OK);

	length = WideCharToMultiByte(CP_ACP, 0, wszPath, -1, NULL, 0, NULL, NULL);
	szPathLocalLow = (char*)malloc(sizeof(char) * length);
	ret = WideCharToMultiByte(CP_ACP, 0, wszPath, -1, szPathLocalLow, length, 0, 0);
	
	//MessageBox(NULL, szPathLocalLow, "LocalLow", MB_OK);

	/*
	strcpy_s(szPath, MAX_PATH, szPathLocalLow);
	strcat_s(szPath, MAX_PATH, CONF_PATH);
	strcat_s(szPath, MAX_PATH, "\\default.conf");
	*/
	
	szTemp = getenv("windir");
	szPathTemp[0] = szTemp[0];
	szPathTemp[1] = szTemp[1];
	//strcpy_s(szPath, MAX_PATH, "C:");
	szTemp = getenv("homepath");
	strcat_s(szPathTemp, MAX_PATH, szTemp);
	strcat_s(szPathTemp, MAX_PATH, "\\AppData\\LocalLow\\Safehard\\conf\\default.conf");

	MessageBox(NULL, szPathTemp, "windows dir", MB_OK);
	
	SetTrayConfig(szPath, "close");

	return 0;
}


int SetTrayConfig(const char* path, const char* value)
{
	int line = 0, i, j;
	int length, len_key = 0;
	int ret = 0;
	char config[10][100] = { {0, }, };
	char key[] = "SafeHardUSB";
	char new_value[64] = {0, };
	FILE* pFile = NULL;

	len_key = strlen(key);

	//MessageBox(NULL, path, "config file", MB_OK);


	// read conf
	pFile = fopen(path, "r");
	if (pFile == NULL) {
		return -1;
	}

	while (fgets(config[line], 100, pFile) != NULL) {
		++line;
	}
	fclose(pFile);

	for (i = 0; i < line; i++) {
		// find usb conf
		length = strlen(config[i]);
		if (len_key > length) {
			continue;
		}
		for (j = 0; j < len_key; j++) {
			if (config[i][j] != key[j])
				break;
		}
		if (j == len_key) {
			break;
		}
	}

	if (i < line) {
		// found 
		strcpy_s(new_value, 64, key);
		strcat_s(new_value, 64, "=");
		strcat_s(new_value, 64, value);
	
		length = strlen(new_value);
		new_value[length] = '\0';

		// save conf
		pFile = fopen(path, "w");
		if (pFile == NULL) {
			return -1;
		}

		for (j = 0; j < line; j++) {
			if (i == j)
				fprintf(pFile, "%s\n", new_value);
			else
				fprintf(pFile, "%s", config[j]);
		}
		fclose(pFile);
	}

	return 0; 
}

