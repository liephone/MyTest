#include <iostream>
using namespace std;

const char* p = "Heraclitus";
const char* q = "Heraclitus";

int main()
{
	if (p == q)
		cout << "same" << endl;
	else
		cout << "difference" << endl;

	char* ps = "Plato";
	ps[4] = 'e';

	return 0;
}

