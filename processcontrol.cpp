


#include <windows.h>
#include <stdio.h>
#include <Shellapi.h>	// Shell32.lib
#include <string>
						// user32.lib


using namespace std;

int main()
{
	string tray = "D:\\Project_src\\SafeHard\\trunk\\client\\package\\win\\bin\\safehardtray.exe";

	// ShellExecute
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb762153(v=vs.85).aspx
	//HINSTANCE nResult = ::ShellExecuteA(NULL, "open", tray.c_str(), NULL, NULL, SW_SHOW);


	HWND hWnd = ::FindWindowA(NULL, "safehardtray");
	if (hWnd == NULL) {
		printf("not found..\n");
	}
	DWORD dwPid = 0;
	DWORD dwRet = ::GetWindowThreadProcessId(hWnd, &dwPid);

	printf("pid: [%d], ret: [%d]\n", dwPid, dwRet);


	// GetExitCodeProcess
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms683189(v=vs.85).aspx

	DWORD dwExitCode = 0;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, //PROCESS_QUERY_INFORMATION
			FALSE, 
			dwPid
	);
	if (hProcess) {
		BOOL bRet = ::GetExitCodeProcess(hProcess, &dwExitCode);
		if (bRet == TRUE) {
			printf("exitcode : [%d]\n", dwExitCode);
		} else {
			printf("GetExitCodeProcess failed..\n");
		}

		CloseHandle(hProcess);
	}
	

	/*
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 
			FALSE, 
			dwPid
	);
	if (hProcess) {
		// TerminateProcess
		// http://msdn.microsoft.com/ko-kr/library/windows/desktop/ms686714(v=vs.85).aspx
		BOOL bTerminated = TerminateProcess(hProcess, 0);
		if (bTerminated) {
			printf("terminated...\n");
		} else {
			printf("not terminated...\n");
		}

		CloseHandle(hProcess);
	}
	*/



	// ExitProcess
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms682658(v=vs.85).aspx
	::ExitProcess(dwExitCode);

	

	return 0;
}
