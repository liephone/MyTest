#include <iostream>
using namespace std;

int main()
{
	int v1[10];
	int v2[10];
	int i1 = &v1[5] - &v1[3];
	int i2 = &v1[5] - &v2[3];
	char temp[1024][512];

	cout << i1 << endl;
	cout << &v1[5] << endl;
	cout << &v1[3] << endl;
	cout << i1 << endl;
	cout << i2 << endl;


	return 0;
}

