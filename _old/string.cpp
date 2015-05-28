#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
try {
	static string lower = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./";
	string str = " ";
	char test[4] = "12\\";

	cout << lower.size() << endl;
	cout << test << endl;
	cout << str.length() << endl;


	//
	vector<string> DNs;
	DNs.push_back("CN=WIZVERA Corporation, ...... L=Songpa-gu, S=Seoul, C=KR");
	DNs.push_back("CN=WIZVERA CO., LTD, ....ion v2, O=WIZVERA CO., LTD, L=Songpa-gu, S=Seoul, C=KR");


	for (int i = 0; i < DNs.size(); i++) {
		cout << DNs.at(i).c_str() << endl;

		int rt = DNs.at(i).compare(3, 7, "WIZVERA");
		if (rt == 0)
			cout << 
	}

	return 0;
}
catch (out_of_range) {
	cerr << "range error";
}
catch (...) {
	cerr << "unknown exception thrown\n";
}
