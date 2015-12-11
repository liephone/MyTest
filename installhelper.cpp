#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <Shlobj.h>
#include <Shellapi.h>
#include <io.h>
#include <string>
#include <vector>
using namespace std;

#pragma comment (lib, "Shell32.lib")
#pragma comment (lib, "User32.lib")

bool getRecommentPortNames(vector<string>& portnames, const vector<string>& com0list);
bool getAvailablePortNames(vector<string>& portnames);
bool getEmptyPortNames(vector<string>& portnames, const vector<string>& com0list);
int findStringInVector(const string& str, const vector<string> ar);


int main(int argc, char* argv[]) 
{
	// arguments
	// - 0: InstallHelper
	// - 1: destination symbol 
	// - 2: destination path 
	// - 3: destination parameter
	if (argc < 2 || argc > 4) {
		printf("Usage: InstallHelper <target symbol> <target path> <target parameter> \n");
		printf("\n");
		printf(" <symbol> : com (com0com), ... \n");
		printf(" <path> : install file path \n");
		printf(" <parameter> : intall parameter \n");
		return 1;
	}
	string dest = argv[1];
	string path = "";
	string param = "";
	if (argc == 3) {
		path = argv[2];
	}
	if (argc == 4) {
		path = argv[2];
		param = argv[3];
	}

	printf("target symbol : %s\n", dest.c_str());
	printf("target path : %s\n", path.c_str());
	printf("target param  : %s\n", param.c_str());

	// program section
	// - 0: com (com0com)
	// - 1: 

	if (dest.compare("com") == 0) {
		// check installed..
		//  - get program path (PGF/com0com/)
		char szComPath[MAX_PATH] = {0, };
		char szSetupc[MAX_PATH] = {0, };
		char szSetupcList[MAX_PATH] = {0, };
		HRESULT hr = SHGetSpecialFolderPathA(NULL, szComPath, CSIDL_PROGRAM_FILES, TRUE);
		if (!SUCCEEDED(hr)) {
			return false;
		}
		strcat_s(szComPath, MAX_PATH, "\\com0com");
		strcpy_s(szSetupc, MAX_PATH, szComPath);
		strcat_s(szSetupc, MAX_PATH, "\\setupc.exe");
		
		printf("%s\n", szComPath);
		printf("%s\n", szSetupc);

		if (_access(szSetupc, 0) != -1) {
			// - already installed
			//   - setupc list
			//   - setupc ...
			
			char szCurPath[MAX_PATH] = {0, };
            ::GetCurrentDirectoryA(sizeof(szCurPath), szCurPath);
            ::SetCurrentDirectoryA(szComPath);

			string cmd = "setupc list";

			vector<string> lists;
			FILE* pipe = _popen(cmd.c_str(), "r");
			if (pipe != NULL) {
				char buffer[512] = {0, };
				while (!feof(pipe)) {
					if (fgets(buffer, 512, pipe) != NULL) {
						string result = buffer;
						lists.push_back(result);
					}
				}
				_pclose(pipe);
			}

			
			vector<string> recommendList;
			getRecommentPortNames(recommendList, lists);

			//test
			for (int i = 0; i < recommendList.size(); i++) {
				printf("%s\n", recommendList.at(i).c_str());
			}

            ::SetCurrentDirectoryA(szCurPath);

		} else {
			// - not installed
			//   - register environment variables 
			//     - CNC_INSTALL_CNCA0_CNCB0_PORTS : YES
			//     - CNC_INSTALL_COMX_COMX_PORTS : YES
			
			//   - shellexecute setup_com0com_W7_x64_signed.exe /S
			SHELLEXECUTEINFOA execinfo;
			ZeroMemory(&execinfo, sizeof(execinfo));
			execinfo.cbSize = sizeof(execinfo);
			execinfo.lpVerb = "runas";
			execinfo.lpFile = path.c_str();
			execinfo.lpParameters = "/S";
			execinfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
			execinfo.nShow = SW_SHOW;

			int r = (int)ShellExecuteExA(&execinfo);
			WaitForSingleObject(execinfo.hProcess, INFINITE);
		}

	}

	return 0;
}

bool getRecommentPortNames(vector<string>& portnames, const vector<string>& com0list)
{
	vector<string> com0names;
	
	for (int i = 0; i < com0list.size(); i++) {
		// 0 ~ 9 (48 ~ 57)
		string line = com0list.at(i);
		size_t pos = line.rfind("COM");
		if (pos == string::npos) {
			continue;
		}
		string name = "";

		if (line.size() > pos + 4) {
			if (line.at(pos + 4) > 47 && line.at(pos + 4) < 58) {
				name = line.substr(pos, 5);
			} else {
				name = line.substr(pos, 4);
			}
		} else {
			name = line.substr(pos, 4);
		}

		if (!name.empty()) {
			com0names.push_back(name);
		}
	}
	if (getEmptyPortNames(portnames, com0names)) {
		printf("recommend success..\n");
	} else {
		printf("recommend failed..\n");
	}

	return true;
}

bool getAvailablePortNames(vector<string>& portnames)
{
	try {
		for (int i = 1; i <= 255; i++) {
			char szPortName[32] = {0, };
			sprintf_s(szPortName, "\\\\.\\COM%d", i);
			HANDLE hSerial = CreateFileA(szPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
			if (hSerial == INVALID_HANDLE_VALUE) {
				::CloseHandle(hSerial);
				hSerial = 0;
				continue;
			}
			::CloseHandle(hSerial);
			hSerial = 0;
			string port = szPortName + 4;
			portnames.push_back(port);
		}
	} catch (...) {
		return false;
	}
	return true;
}

bool getEmptyPortNames(vector<string>& portnames, const vector<string>& com0list)
{
	try {
		for (int i = 1; i <= 255; i++) {
			char szPortName[32] = {0, };
			sprintf_s(szPortName, "\\\\.\\COM%d", i);
			HANDLE hSerial = CreateFileA(szPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
			if (hSerial == INVALID_HANDLE_VALUE) {
				string port = szPortName + 4;
				int ret = findStringInVector(port, com0list);
				if (ret < 0) {
					portnames.push_back(port);
				}
				if (portnames.size() >= 20) {
					::CloseHandle(hSerial);
					hSerial = 0;
					break;
				}
			}
			::CloseHandle(hSerial);
			hSerial = 0;
		}
	} catch (...) {
		return false;
	}
	return true;
}

int findStringInVector(const string& str, const vector<string> ar)
{
	int index = -1;

	for (int i = 0; i < ar.size(); i++) {
		if (ar.at(i).compare(str) == 0) {
			index = i;
		}
	}

	return index;
}

