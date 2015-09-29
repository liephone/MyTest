
#include "StringUtil.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

using namespace std;

const string Whitespace="\r\n\t ";

int StringUtil::parseInt(const string& str)
{
	return atoi(str.c_str());
}

long StringUtil::parseLong(const string& str)
{
	return atol(str.c_str());
}

string StringUtil::trimLeft(const string& str)
{
	string::size_type n = str.find_first_not_of(Whitespace);
	return n == string::npos ? str : str.substr(n, str.length());
}

string StringUtil::trimRight(const string& str)
{
	string::size_type n = str.find_last_not_of(Whitespace);
    return n == string::npos ? str : str.substr(0, n + 1);
}

string StringUtil::trim(const string& str)
{
	return trimLeft(trimRight(str));
}

vector<string> StringUtil::splitAndTrim(const string& str, char delim, bool removeEmpty)
{
	vector<string> result;
	string::size_type cutAt, pos=0;

	while ((cutAt = str.find_first_of(delim, pos)) != str.npos) {
		string s = StringUtil::trim(str.substr(pos, cutAt-pos));
		if (!removeEmpty || s.size() > 0)
			result.push_back(s);
		pos = cutAt + 1;
	}

	string s = trim(str.substr(pos));
	if (!removeEmpty || s.size() > 0) {
		result.push_back(s);
	}

	return result;
}

vector<string> StringUtil::split(const string& str, char delim, bool removeEmpty) 
{
	vector<string> result;
	string::size_type cutAt, pos=0;

	while ((cutAt = str.find_first_of(delim, pos)) != str.npos) {
		string s = str.substr(pos, cutAt-pos);
		if (!removeEmpty || s.size() > 0)
			result.push_back(s);
		pos = cutAt + 1;
	}

	string s = str.substr(pos);
	if (!removeEmpty ||s.size() > 0) {
		result.push_back(s);
	}

	return result;
}

wstring StringUtil::strToWstr(const string &s)
{
	int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	return r;
}

string StringUtil::wstrToStr(const wstring &s)
{
	int slength = (int)s.length() + 1;

	int len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);

	return r;
}

wstring StringUtil::utf8ToWide(const string& str)
{
	const char *pszCode = str.c_str();

	int nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, str.length(), NULL, NULL);
	BSTR bstrWide = SysAllocStringLen(NULL, nLength);
	MultiByteToWideChar(CP_UTF8, 0, pszCode, str.length(), bstrWide, nLength);

	wstring wret = wstring(bstrWide);
	SysFreeString(bstrWide);

	return wret;
}

string StringUtil::utf8ToEuckr(const string& str)
{
	const char *pszCode = str.c_str();

	int nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, str.length(), NULL, NULL);
	BSTR bstrWide = SysAllocStringLen(NULL, nLength);

	MultiByteToWideChar(CP_UTF8, 0, pszCode, str.length(), bstrWide, nLength);
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	char* pszAnsi = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);

	SysFreeString(bstrWide);

	return string(pszAnsi);
}

string StringUtil::euckrToUtf8(const string& str)
{
	const char *pszCode = str.c_str();

	int inputLen = str.length();
	if (inputLen == 0)
		return "";
	inputLen += 1;

	WCHAR *Unicode = new WCHAR[inputLen];
	char *utf8 = new char[inputLen * 3];

	memset(Unicode, 0, sizeof(WCHAR) * inputLen);
	memset(utf8, 0, sizeof(char) * inputLen);

	int nUnicodeSize = MultiByteToWideChar(CP_ACP, 0, pszCode, strlen(pszCode), Unicode, inputLen);
	int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, Unicode, nUnicodeSize, utf8, sizeof(char) * inputLen * 3, NULL, NULL);
	int error = GetLastError();
	MultiByteToWideChar(CP_UTF8, 0, utf8, nUTF8codeSize, Unicode, sizeof(WCHAR) * inputLen);

	string ret;
	ret.assign(utf8,nUTF8codeSize);

	if (Unicode != NULL)
		delete Unicode;
	if (utf8 != NULL)
		delete utf8;

	return ret;
}

