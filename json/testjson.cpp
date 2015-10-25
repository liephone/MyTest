#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <json/json.h>
using namespace std;


#pragma comment(lib, "json.lib")


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

	//wstring input1 = L"{\n \"success\": \"0000\",\n \"result\" : {\n\t \"message\"    : \"[success] update information\",\n\t \"updateno\"   : \"UPDATE20150921000017\",\n\t \"updatename\" : \"POS UPDATE TEST3\",\n\t \"version\"    : \"1.0.0.1\",\n\t \"description\": \"pos update test3 descrioption\"\n\t }\n}";
	//printf("%s\n", input1.c_str());

	string input2 = "{\n \"success\" : \"0000\",\n \"message\" : \"[success] check\"\n}";
	printf("%s\n", input2.c_str());

	//string input3 = "{\n \"success\" : \"0000\",\n \"message\" : \"update file list\",\n \"result\" : {\n \"itemcnt\" : "4",\n \"items\" : [\n { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000006\", \"filename\":\"C272F22B7B624CFE9AD6B615DF02D500.png\", \"filepath\":\"\\\", \"filesize\":\"229957\"},\n { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000008\", \"filename\":\"3273FF4EFAB44C1B8801740DF7EA52F6.png\", \"filepath\":\"\\\", \"filesize\":\"192838\"},\n { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000010\", \"filename\":\"EE8DB98D4CE646AF877D9ED8BFDBC377.png\", \"filepath\":\"\\\", \"filesize\":\"68370\"},\n { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000011\", \"filename\":\"B9FC6F0CBA3A4776B1F5CC6240AC41E6.png\", \"filepath\":\"\\\", \"filesize\":\"164925\"}\n ]\n }\n }";
	//printf("%s\n", input3.c_str());

	//string input4 = "{\n \"success\" : \"0000\",\n \"message\" : \"update file list\",\n \"result\" : {\n \"itemcnt\" : \"4\",\n \"items\" : [\n { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000006\", \"filename\":\"C272F22B7B624CFE9AD6B615DF02D500.png\", \"filepath\":\"\\\", \"filesize\":\"229957\"},\n { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000011\", \"filename\":\"B9FC6F0CBA3A4776B1F5CC6240AC41E6.png\", \"filepath\":\"\\\", \"filesize\":\"164925\"}\n ]\n }\n }";
	//printf("%s\n", input4.c_str());

	string input5 = "{  \"success\" : \"0000\", \"message\" : \"update file list\", \"result\" : {              \"itemcnt\" : \"4\", \"items\" : [                           { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000006\", \"filename\":\"C272F22B7B624CFE9AD6B615DF02D500.png\", \"filepath\":\"\\\", \"filesize\":\"229957\"}, { \"updateno\":\"UPDATE20150921000017\", \"fileno\":\"UPDATEFILE20150921000000000011\", \"filename\":\"B9FC6F0CBA3A4776B1F5CC6240AC41E6.png\", \"filepath\":\"\\\", \"filesize\":\"164925\"}                          ]             }}";

	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(input5, root)) {
		printf("\nError[%d], Cannot load json data !!!\n", -3001);
		return -3001;
	}

	try {
		string success = root["success"].asString();
		string message = root["message"].asString();

		printf("success : %s\n", success.c_str());
		printf("message : %s\n", message.c_str());
	} catch (...) {
		printf("\nError[%d], Cannot access specific item\n", -3002);
		return -3002;
	}

	return 0;
}
