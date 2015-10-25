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
	string url_check = "http://pos.cocobri.co.kr/connect/check.do";
	string url_update = "http://pos.cocobri.co.kr/update/check.do";
	string url_updatelist = "http://pos.cocobri.co.kr/update/file/list.do?updateno=UPDATE20150921000017";
	string url_login = "http://pos.cocobri.co.kr/connect/login.do";
	string url_logout = "http://pos.cocobri.co.kr/connect/logout.do";
	string url_list = "http://pos.cocobri.co.kr/standby/list.do";
	string url_calllist = "http://pos.cocobri.co.kr/standby/list.do?storecode=STORE201509150000003&callstate=10&calltype=PR";

	map<string, string> update_params;
	string updateno = "UPDATE20150921000017";
	update_params["updateno"] = updateno;

	map<string, string> login_params;
	string userid = "kimkisun";
	string userpw = "kimkisun";
	login_params["userid"] = userid;
	login_params["userpw"] = userpw;

	map<string, string> logout_params;
	string usercode = "SUSER201509210000001";
	string storecode = "STORE201509150000003";
	logout_params["userid"] = userid;
	logout_params["usercode"] = usercode;
	logout_params["storecode"] = storecode;

	map<string, string> list_params;
	string callstate = "10";
	string calltype = "PR";
	login_params["storecode"] = storecode;
	login_params["callstate"] = callstate;
	login_params["calltype"] = calltype;

	WinINet client;

	//string res = client.Post(url_login, login_params);
	//printf("res : [%s]\n", res.c_str());
	
	//string res = client.Post(url_logout, logout_params);
	//printf("res : [%s]\n", res.c_str());

	//Sleep(1000);

	//string list = client.Post(url_list, list_params);
	//printf("res : [%s]\n", list.c_str());
	
	string list = client.Get(url_calllist);
	printf("res : [%s]\n", list.c_str());

	//string res = client.Get(url_check);
	//printf("res : [%s]\n", res.c_str());

	//string res_update = client.Get(url_update);
	//printf("res update : [%s]\n", res_update.c_str());
	
	string res = client.Get(url_updatelist);
	printf("res : [%s]\n", res.c_str());


	return 0;
}

