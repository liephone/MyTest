#include <stdio.h>
#include <string>
using namespace std;



int main()
{
	string res = "{\"preConnRemovableDisk\":\"false\",\"removableDiskEnable\":\"false\"}";

	printf("res: [%d] %s\n", res.size(), res.c_str());

	string key1 = "preConnRemovableDisk";
	string key2 = "removableDiskEnable";

	size_t found = res.find(key1);
	if (found == string::npos) {
		printf("invalid config !!!\n");
	}
	printf("pos : %d\n", found);

	string value1 = res.substr(found + key1.size() + 3, 4);
	printf("key1 : value1 = %s : %s \n", key1.c_str(), value1.c_str());

	found = res.find(key2);
	if (found == string::npos) {
		printf("invalid config !!!\n");
	}
	printf("pos : %d\n", found);

	string value2 = res.substr(found + key2.size() + 3, 4);
	printf("key2 : value2 = %s : %s \n", key2.c_str(), value2.c_str());


	return 0;
}
