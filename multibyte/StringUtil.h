#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <string>
#include <vector>


class StringUtil 
{
public:
	static int parseInt(const std::string& str);
	static long parseLong(const std::string& str);

	static std::string trimLeft(const std::string& str);
	static std::string trimRight(const std::string& str);
	static std::string trim(const std::string& str);

	static std::vector<std::string> splitAndTrim(const std::string& str, char delim, bool removeEmpty);
	static std::vector<std::string> split(const std::string& str, char delim, bool removeEmpty);

	static std::wstring StringUtil::strToWstr(const std::string &s);
	static std::string StringUtil::wstrToStr(const std::wstring &s);
	static std::wstring StringUtil::utf8ToWide(const std::string& str);
	static std::string StringUtil::utf8ToEuckr(const std::string& str);
	static std::string StringUtil::euckrToUtf8(const std::string& str);

};


#endif//_STRINGUTIL_H
