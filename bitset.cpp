#include <iostream>
#include <stdio.h>
#include <string>
#include <bitset>
using namespace std;

int main()
{
	bitset<16> dots;
	bitset<16> foo;
	bitset<16> bar(0xfa2);
	bitset<16> baz(string("0101111001"));

	cout << "dots: " << dots << '\n';
	cout << "foo: " << foo << '\n';
	cout << "bar: " << bar << '\n';
	cout << "baz: " << baz << '\n';

	return 0;
}

