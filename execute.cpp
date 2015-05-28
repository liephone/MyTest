#include "stdafx.h"
#include <windows.h>
#include "resource.h"
#include <tchar.h>
#include <shellapi.h>	// shell32.lib
#include <string>
#include <stdio.h>
using namespace std;

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")

BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam); 
HINSTANCE hInst;

int main()
{
	//string cmd = "C:\\Program Files (x86)\\Wizvera\\Safehard\\safehardtray.exe";
	//string param = "close";
	//
	//wchar_t cmd[256] = L"C:\\Program Files (x86)\\Wizvera\\Safehard\\safehardtray.exe";
	//wchar_t param[64] = L"close";

	//HINSTANCE nResult = ::ShellExecuteW(NULL, L"open", cmd, param, NULL, SW_SHOW);
	
	//char fileName[MAX_PATH] = {0, };

	//GetModuleFileName(NULL, fileName, MAX_PATH);

	//printf("path: %s\n", fileName);

	//char cmd[256] = "safehardtray.exe close";
	//FILE* pFile = _popen(cmd, "r");
	//
	
	//HINSTANCE nResult = ::ShellExecuteW(NULL, L"open", L"dokanctl.exe", L"/r s", NULL, SW_HIDE);

	SHELLEXECUTEINFOA execinfo;
    ZeroMemory( &execinfo, sizeof(execinfo) );
    execinfo.cbSize = sizeof(execinfo);
    execinfo.lpVerb = "open";
    execinfo.lpFile = "dokanctl.exe";
    execinfo.lpParameters = "/r s";
    execinfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
    execinfo.nShow = SW_HIDE;

	MSG msg;
	HMODULE hAppInst = GetModuleHandle(NULL);
	HWND hWnd = ::CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
	::ShowWindow(hWnd, SW_SHOW);
		
	BOOL bRet;
while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) 
{ 
    if (bRet == -1)
    {
        // Handle the error and possibly exit
    }
    else if (!IsWindow(hWnd) || !IsDialogMessage(hWnd, &msg)) 
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    } 
} 

	

    //int r = (int)ShellExecuteExA( &execinfo );
    //WaitForSingleObject(execinfo.hProcess, INFINITE);

	//printf("executed complete..\n");

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch (message) 
    { 
	case WM_INITDIALOG: 
		break; 
		
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		case IDOK: 
		case IDCANCEL: 
			DestroyWindow(hwndDlg); 
			return TRUE; 
		} 
    } 
    return FALSE; 
} 

