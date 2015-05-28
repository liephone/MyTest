#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

int main()
{
	string str = "http://192.168.0.123:8080/safeHardRelayServer/safeHardReq.do";

	char buff[256] = {0, };
	sprintf(buff, "url [%s]", str.c_str());
	printf("%s\n", buff);

	return 0;
}

