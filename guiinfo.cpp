#include <windows.h>
#include <stdio.h>

int main()
{
	DWORD pid = 3740;

	GUITHREADINFO info;
	ZeroMemory(&info, sizeof(info));
	info.cbSize = sizeof(GUITHREADINFO);

	BOOL bRet = GetGUIThreadInfo(NULL, &info);
	if (bRet) {
		printf("get info ok....\n");

		printf("flags: %d, HWNDactive: %d, HWNDfocus: %d, ...\n", 
				(int)info.flags, (int)info.hwndActive, (int)info.hwndFocus);

		DWORD dwPid = 0;
		DWORD dwRet = GetWindowThreadProcessId(info.hwndActive, &dwPid);
		
		printf("pid: %d, ret: %d \n", dwPid, dwRet);

	} else {
		int err = GetLastError();
		printf("get info fail [%d] !!!\n", err);
	}

	//HWND hWndTemp = (HWND)0x000005f0;
	HWND hWndTemp = (HWND)525796;
	DWORD dwPid2 = 0;
	DWORD dwTemp = GetWindowThreadProcessId(hWndTemp, &dwPid2);

	printf("handle pid : %d\n", dwPid2);

	return 0;
}
