#include <windows.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <wchar.h>
using namespace std;

wstring s2ws(const string& s);

int main()
{

	string str = "cn=김기선(Kim Ki Sun)0004042702050694,ou=KMB,ou=personal4IB,o=yessign,c=kr";
	wstring wstr = L"cn=김기선(Kim Ki Sun)0004042702050694,ou=KMB,ou=personal4IB,o=yessign,c=kr";

	string tmp;
	tmp.assign(wstr.begin(), wstr.end());

	// s -> w
	int len;
	int slength = (int)str.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	std::wstring r(buf);

	wstring wstrBuf = s2ws(str);

	if (str == tmp)
		cout << "=" << endl;
	else 
		cout << "diff." << endl;
	
	if (wstr == wstrBuf)
		cout << "=" << endl;
	else 
		cout << "diff." << endl;

	cout << str << endl;
	cout << wstr.size() << endl;
	cout << wstrBuf.size() << endl;
	cout << wcslen(wstr.c_str()) << endl;
	cout << wcslen(wstr.c_str()) * sizeof(wchar_t) << endl;
	wcout << wstrBuf.c_str() << endl;
	wcout << r.c_str() << endl;
	wcout << buf << endl;

	return 0;
}


wstring s2ws(const string& s) 
{
    std::string curLocale = setlocale(LC_ALL, ""); 
    const char* _Source = s.c_str();
    size_t _Dsize = mbstowcs(NULL, _Source, 0) + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}
