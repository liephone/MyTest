#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <json/json.h>
using namespace std;

#pragma comment(lib, "json.lib")


int main()
{
	printf("test json parser...\n\n");

	/*
	string input1 = "{ \"success\": \"0000\",
		   "result" : { \"message\"    : \"[성공] 업데이트 정보\",
			   \"updateno\"   : \"UPDATE20150921000017\",
			   \"updatename\" : \"포스 업데이트 테스트3\",
			   \"version\"    : \"1.0.0.1\",
			   \"description\": \"포스 업데이트 테스트3 내용\" } }";
	*/

	string input1 = "{\n \"success\": \"0000\",\n \"result\" : {\n\t \"message\"    : \"[success] update information\",\n\t \"updateno\"   : \"UPDATE20150921000017\",\n\t \"updatename\" : \"POS UPDATE TEST3\",\n\t \"version\"    : \"1.0.0.1\",\n\t \"description\": \"pos update test3 descrioption\"\n\t }\n}";

	string test = "{\"p_version\":\"0.0.1\",\"version\":\"0.0.1\",\"code\":100,\"msg\":\"success\",\"data\":[{\"s_name\":\"Wesaladu\",\"mem_email\":\"lovesun19@naver.com\",\"s_number\":1102078254,\"s_addr\":\"Jeju Seoguipo 82\",\"orderdate\":\"2015-11-04T15:00:00.000Z\",\"s_ceoname\":\"Kim Tae Gyoem\",\"ordernum\":\"20151109_1\",\"country_num\":86,\"total_price\":\"200\",\"menu_data\":[{\"idx\":1,\"m_name\":\"ice americano\",\"m_price\":4000,\"m_size\":\"Standard\",\"m_size_price\":0,\"m_option\":\"test\",\"m_option_price\":0,\"m_ea\":\"3\"},{\"idx\":2,\"m_name\":\"cafe latte\",\"m_price\":3000,\"m_size\":\"Large\",\"m_size_price\":500,\"m_option\":\"test\",\"m_option_price\":0,\"m_ea\":\"3\"},{\"idx\":1,\"m_name\":\"cafe moca\",\"m_price\":4000,\"m_size\":\"Large\",\"m_size_price\":500,\"m_option\":\"test\",\"m_option_price\":0,\"m_ea\":\"3\"}]}]}";


	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(test, root)) {
		printf("\nError[%d], Cannot load json data !!!\n", -3001);
		return -3001;
	}

	try {
		// basic info.
		string p_version = root["p_version"].asString();
		int icode = root["code"].asInt();
		char buff[32] = {0, };
		string code = itoa(icode, buff, 10);
		string msg = root["msg"].asString();

		printf("code : %s\n", code.c_str());
		printf("msg : %s\n", msg.c_str());
		printf("p_version : %s\n\n", p_version.c_str());

		// data
		Json::Value data = root["data"];
		if (!data.empty()) {
			int count = data.size();
			printf("data count : %d\n", count);

			for (int i = 0; i < count; i++) {
				Json::Value::Members keys = data[i].getMemberNames();
				printf("keys count : %d\n", keys.size());

				map<string, string> datamap;
				for (int j = 0; j < keys.size(); j++) {
					string val = "";
					if (data[i].get(keys.at(j), 100).isInt()) {
						char buff[32] = {0, };
						int iVal = data[i].get(keys.at(j), 100).asInt();
						val = itoa(iVal, buff, 10);
					} else if (data[i].get(keys.at(j), 100).isString()) {
						val = data[i].get(keys.at(j), 100).asString();
					} else if (data[i].get(keys.at(j), 100).isArray()) {
						printf("\n");

						val = "[array]";
						// menu_data
						Json::Value menus = data[i][keys.at(j)];
						int menu_count = menus.size();
						printf("\tmenu_count : %d\n", menu_count);

						for (int m = 0; m < menu_count; m++) {
							Json::Value::Members mkeys = menus[m].getMemberNames();
							printf("\tmenu keys count : %d\n", mkeys.size());

							map<string, string> menumap;
							for (int n = 0; n < mkeys.size(); n++) {
								string mval = "";
								if (menus[m].get(mkeys.at(n), 100).isInt()) {
									char buff2[32] = {0, };
									int iVal2 = menus[m].get(mkeys.at(n), 100).asInt();
									mval = itoa(iVal2, buff2, 10);
								} else if (menus[m].get(mkeys.at(n), 100).isString()) {
									mval = menus[m].get(mkeys.at(n), 100).asString();
								} 
								printf("%s : %s\n", mkeys.at(n).c_str(), mval.c_str());
								//menumap[mkeys.at(n)] = mval;
							}
							//m_menudata.push_back(menumap);
						}
					}
					printf("%s : %s\n", keys.at(j).c_str(), val.c_str());
					//datamap[keys.at(j)] = val;
				}
				//m_data.push_back(datamap);
			}
		}

	} catch (...) {
		printf("\nError[%d], Cannot access specific item\n", -3002);
		return -3002;
	}

	return 0;
}

