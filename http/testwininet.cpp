#include <windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include "WinINetClient.h"
#include "LogUtil.h"

using namespace std;

int main()
{
	LogUtil logger("testwininet");

	string url_check = "http://pos.cocobri.co.kr/connect/check.do";
	string url_update = "http://pos.cocobri.co.kr/update/check.do";

	logger.Log("check url : [%s]", url_check.c_str());
	logger.Log("update check url : [%s]", url_update.c_str());

	WinINet client;
	string res = client.Get(url_check);
	logger.Log("client.get(check) : [%s]", res.c_str());
	printf("res : [%s]\n", res.c_str());

	Sleep(100);
	printf("sleep .");
	Sleep(100);
	printf(".");
	Sleep(100);
	printf(".");
	Sleep(100);
	printf("\n\n");

	string res_update = client.Get(url_update);
	logger.Log("client.get(update) : [%s]", res_update.c_str());
	printf("res update : [%s]\n", res_update.c_str());

	res = client.Get(url_check);
	res = client.Get(url_check);
	res = client.Get(url_check);

	return 0;
}

