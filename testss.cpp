#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

#define KEY (32)

int main()
{
	string str = "test";
	string data = str.c_str() + KEY;

	printf("[%s]\n", data.c_str());
	cout << data << endl;

	return 0;
}
