#include <iostream>
#include <string>
using namespace std;

int main()
{
	string str = "C:/Users/wiz/AppData/LocalLow/NPKI/yessign/User/ou=KMB,ou=personal4IB,o=yessign,c=kr/signCert.der";
	string tmp = "";

	unsigned found = str.rfind('/');
	if (found != string::npos) {
		tmp = str.substr(0,found);
	}

	cout << tmp << endl;

	return 0;
}

