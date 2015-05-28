#include <afx.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <Shellapi.h>
using namespace std;


int main()
{
	CString strInstCode = _T("Wizvera");
	CString strInflowDomain = _T("kbstar");

	CString strUrl;
	strUrl.Format(_T("https://www.safehard.co.kr/register.do?inflowInstCode=%s&inflowDomain=%s"), strInstCode, strInflowDomain);
	CString strUrl2;
	strUrl2.Format(_T("https://www.safehard.co.kr/register.do?inflowInstCode=%s"), strInstCode);
	CString strUrl3 = _T("https://www.safehard.co.kr/register.do");

	cout << strUrl << endl;

	//::ShellExecute(NULL, _T("open"), _T("iexplore.exe"), (LPCTSTR)strUrl, NULL, SW_SHOWNORMAL);
	//::ShellExecute(NULL, _T("open"), _T("iexplore.exe"), (LPCTSTR)strUrl2, NULL, SW_SHOWNORMAL);
	//::ShellExecute(NULL, _T("open"), _T("iexplore.exe"), (LPCTSTR)strUrl3, NULL, SW_SHOWNORMAL);

	char szDomain[] = "kbstar";

	string str;
	str = szDomain;
	cout << szDomain << endl;

	return 0;
}

