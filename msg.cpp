#include <windows.h>
#include <stdio.h>
#include <string>
using namespace std;

int main()
{
	//::MessageBoxA(NULL, "https://www.safehard.co.kr", "login success", MB_OK);
	//HWND SysLink;
	//SysLink = CreateWindowExA(0, (LPCSTR)WC_LINK,  "Produced by <A HREF=\"https://www.safehard.co.kr\">click test</A>", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, NULL, GetModuleHandle(NULL), NULL);
	
	static const TCHAR lpctszTitle[] = TEXT("Stuff"), lpctszClass[] = TEXT("StuffClass");

	HWND hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, lpctszClass,
			lpctszTitle, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
			CW_USEDEFAULT, 0, 250, 55, NULL, NULL, NULL, NULL);


	ShowWindow(hWnd, SW_SHOW);

	getchar();

	DestroyWindow(hWnd);

	return 0;
}
