
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <string>
#include <vector>
using namespace std;

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "user32.lib")

void GetWindowInfo(DWORD pid, HWND &hwnd);
void TerminateProcess(DWORD pid, HWND hwnd);

static BOOL CALLBACK fnEnumWindow(HWND hWnd, LPARAM param) 
{
	if (hWnd == NULL) {
		return TRUE;
	}

	DWORD dwPid = 0;
	GetWindowThreadProcessId(hWnd, &dwPid);
	
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	if (hProcess) {
		char buff[MAX_PATH] = {0, };
		string basename = "";
		if (GetModuleBaseName(hProcess, 0, buff, MAX_PATH)) {
			printf("%d: %s\n", dwPid, buff);
			basename = buff;
			if (basename == "hub4com.exe") {
				printf("###### pid is founded...%d\n", dwPid);
				vector<HWND> *pList = (vector<HWND> *)param;
				pList->push_back(hWnd);
			}
		} else {
			printf("not found pid.\n");
		}
		CloseHandle(hProcess);

	}

	return TRUE;
}

BOOL EnumWindowsProc(HWND hWnd, long lParam)
{
	char buff[255] = {0, };
	char nameofclass[255] = {0, };

	GetWindowText(hWnd, buff, 254);
	GetClassName(hWnd, nameofclass, 254);

	printf("windows text: %s\n", buff);
	printf("name of class: %s\n", nameofclass);

	return TRUE;
}

int main()
{
	HWND hwnd = NULL;
	DWORD pid = 0;

	GetWindowInfo(pid, hwnd);
	if (hwnd == NULL) {
		printf("GetWindowInfo is null !!!\n");
	} else {
		printf("GetWindowInfo found...\n");
		TerminateProcess(pid, hwnd);
		printf("TerminateProcess...\n");
	}

	return 0;
}

void GetWindowInfo(DWORD pid, HWND &hwnd)
{
	hwnd = NULL;
	vector<HWND> wndlist;
	EnumWindows(fnEnumWindow, (LPARAM)&wndlist);

	printf("enum result : [%d]\n", wndlist.size());
	printf("\n");

	if (wndlist.size() > 0) {
		hwnd = wndlist[0];
	}

	return;
}

void TerminateProcess(DWORD pid, HWND hwnd) 
{
	if (hwnd != NULL) {
		::PostMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	}
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL) {
		return;
	}

	::TerminateProcess(hProcess,0);
	CloseHandle(hProcess);
}


