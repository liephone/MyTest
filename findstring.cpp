#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

int main()
{
	string str = "C:/Users/wiz/AppData/LocalLow/NPKI/yessign/User/ou=KMB,ou=personal4IB,o=yessign,c=kr/signCert.der";
	string tmp = "";

	string test1 = "(031)123-4567 ======================================== 2015-11-03 14:53:22 ========================================== product unit price count price ------------------------------------------ 01 americano C1002    3,000   1   3,000   02 cafe moca     C1003     5,000    1    5,000   ========================================== tax...   OrderNumber : 0006 ..";

	string test2 = "------------------------------------------ \n01  Americano\n C1002               3,000    1      3,000 \n02  Cafe Moca\n C1003               5,000    1      5,000 \n==========================================";

	/*
	unsigned found = str.rfind('/');
	if (found != string::npos) {
		tmp = str.substr(0,found);
	}
	*/

	// datetime
	size_t pos = test1.find("2015-");
	string datetime = test1.substr(pos, 19);
	
	// product
	size_t spos = test1.find("------");
	size_t epos = test1.find("======", spos);

	string ordercontents = test1.substr(spos, epos - spos);

	string cont1 = "";
	pos = test2.find("\n01 ");
	if (pos != string::npos) {
		size_t wpos = test2.find("\n", pos + 1);
		cont1 = test2.substr(pos + 1, wpos - pos - 1);
	} else {
		cont1 = "empty!!!";
	}

	string cont2 = "";
	pos = test2.find("\n02 ");
	if (pos != string::npos) {
		size_t wpos = test2.find("\n", pos + 1);
		cont2 = test2.substr(pos + 1, wpos - pos - 1);
	} else {
		cont2 = "empty!!!";
	}
	
	string cont3 = "";
	pos = test2.find("\n03 ");
	if (pos != string::npos) {
		size_t wpos = test2.find("\n", pos + 1);
		cont3 = test2.substr(pos + 1, wpos - pos - 1);
	} else {
		cont3 = "empty!!!";
	}


	// ordernumber
	pos = test1.find("OrderNumber");
	string ordernumber = test1.substr(pos, 18);


	//cout << tmp << endl;
	cout << test1 << endl;
	cout << datetime << endl;
	cout << ordernumber << endl;
	cout << test2 << endl;
	//cout << cont1 << endl;
	//cout << cont2 << endl;
	//cout << cont3 << endl;
	printf("cont1 : [%s]\n", cont1.c_str());
	printf("cont2 : [%s]\n", cont2.c_str());
	printf("cont3 : [%s]\n", cont3.c_str());

	return 0;
}

