#ifndef _LOGUTIL_H
#define _LOGUTIL_H

#include <string>
#include <tchar.h>


class LogUtil
{
public:
#ifndef _UNICODE
	LogUtil(const std::string& logname = "mylog");
#else
	LogUtil(const std::wstring& wlogname = L"mylog");
#endif
	~LogUtil() {}

#ifndef _UNICODE
	void Log(const char* fmt, ...);
#else
	void LogW(const wchar_t* fmt, ...);
#endif

private:
#ifndef _UNICODE
	void WriteLog(const char* text);
	std::string GetLogFilePath(const std::string& logname);		// base path : %temp%
#else
	void WriteLogW(const wchar_t* text);
	std::wstring GetLogFilePathW(const std::wstring& wlogname);	// base path : %temp%
#endif

private:
#ifndef _UNICODE
	std::string m_filepath;
#else
	std::wstring m_wfilepath;
#endif
};

#endif//_LOGUTIL_H

