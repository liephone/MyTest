#include <iostream>
#include <string>
#include <string.h>
#include <tchar.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
using namespace std;

int main()
{
	string location = "D:/";
	string url = "http://download.safehard.co.kr/safehard.exe";
	cout << "downloading SafeHard...";

	/*
	HRESULT hr = URLDownloadToFile(NULL, _T("http://download.safehard.co.kr/safehard.exe"), _T("D:\\safehard.exe"), 0, NULL);

	if (hr == S_OK) {
		cout << "Done!!" << endl;
	} else if (hr == E_OUTOFMEMORY) {
		cout << "Failed (out of memory)!!" << endl;
	} else if (hr == INET_E_DOWNLOAD_FAILURE) {
		cout << "Failed (url is invalid)" << endl;
	} else {
		cout << "failed!!!" << endl;
	}
	*/

	TCHAR tszFileName[1024] = {0, };

	HRESULT hr = URLDownloadToCacheFile(NULL, _T("http://download.safehard.co.kr/safehard.exe"), tszFileName, 1024, 0, NULL);
	if (hr == S_OK) {
		cout << "Done!!" << endl;
		cout << tszFileName << endl;
	} else {
		cout << "failed!!!" << endl;
	}
}

