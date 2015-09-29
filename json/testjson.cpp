#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <json/json.h>

using namespace std;

// TODO
// - 한글 변환 (지원)
//


int main()
{
	printf("test json parser...\n");

	/*
	string input1 = "{ \"success\": \"0000\",
		   "result" : { \"message\"    : \"[성공] 업데이트 정보\",
			   \"updateno\"   : \"UPDATE20150921000017\",
			   \"updatename\" : \"포스 업데이트 테스트3\",
			   \"version\"    : \"1.0.0.1\",
			   \"description\": \"포스 업데이트 테스트3 내용\" } }";
	*/

	string input1 = "{\n \"success\": \"0000\",\n \"result\" : {\n\t \"message\"    : \"[success] update information\",\n\t \"updateno\"   : \"UPDATE20150921000017\",\n\t \"updatename\" : \"POS UPDATE TEST3\",\n\t \"version\"    : \"1.0.0.1\",\n\t \"description\": \"pos update test3 descrioption\"\n\t }\n}";

	wstring input1 = L"{\n \"success\": \"0000\",\n \"result\" : {\n\t \"message\"    : \"[success] update information\",\n\t \"updateno\"   : \"UPDATE20150921000017\",\n\t \"updatename\" : \"POS UPDATE TEST3\",\n\t \"version\"    : \"1.0.0.1\",\n\t \"description\": \"pos update test3 descrioption\"\n\t }\n}";
	printf("%s\n", input1.c_str());

	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(input1, root)) {
		printf("\nError[%d], Cannot load json data !!!\n", -3001);
		return -3001;
	}

	try {
		string success = root["success"].asString();
		string message = root["result"].get("message", 100).asString();

		printf("success : %s\n", success.c_str());
		printf("message : %s\n", message.c_str());
	} catch (...) {
		printf("\nError[%d], Cannot access specific item\n", -3002);
		return -3002;
	}

	return 0;
}
