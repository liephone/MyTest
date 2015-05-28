#include <windows.h>
#include <iostream>
#include <wchar.h>
#include <stdio.h>
#include <string>
using namespace std;

int main()
{
	cout << sizeof(BYTE) << endl;
	cout << sizeof(wint_t) << endl;
	cout << sizeof(int) << endl;
	cout << sizeof(wchar_t) << endl;

	if (0x20 == ' ')
		cout << "space" << endl;
	else
		cout << "else" << endl;

	wstring str = L" 123";
	wint_t c = str[0];
	printf("0x%02X\n", c);
	wcout << str << endl;
	str[0] = wchar_t('a');
	wcout << str << endl;

	string str1 = "abcd";
	wstring str2(str1.begin(), str1.end());

	wcout << str2 << endl;


	cout << sizeof(_LONGLONG) << endl;
	cout << sizeof(long) << endl;


	return 0;
}

