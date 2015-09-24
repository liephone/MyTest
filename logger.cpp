#include <windows.h>
#include <iostream>
#include <cstdarg>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
using namespace std;

/*
 * TODO
 * - thread safe
 * - independent on os
 */


//////////////////////////////////////////////////////////////////////////////////////
// class MyLogger
//
class MyLogger
{
public:
#ifndef _UNICODE
	MyLogger(const string& logname = "MobilePosBell");
#else
	MyLogger(const wstring& wlogname = L"MobilePosBell");
#endif
	~MyLogger() {}

#ifndef _UNICODE
	void Log(const char* fmt, ...);
#else
	void LogW(const wchar_t* fmt, ...);
#endif

private:
#ifndef _UNICODE
	void WriteLog(const char* text);
	string GetLogFilePath(const string& logname);
#else
	void WriteLogW(const wchar_t* text);
	wstring GetLogFilePathW(const wstring& wlogname);
#endif

private:
#ifndef _UNICODE
	string m_filepath;
#else
	wstring m_wfilepath;
#endif
};


#ifndef _UNICODE
MyLogger::MyLogger(const string& logname)
{
	m_filepath = GetLogFilePath(logname);
}

void MyLogger::Log(const char* fmt, ...)
{
	char text[MAX_PATH] = {0, };
	va_list args;
	va_start(args, fmt);
	vsprintf_s(text, MAX_PATH, fmt, args);
	va_end(args);

	printf("%s", text);
	WriteLog(text);
}

void MyLogger::WriteLog(const char* text)
{
	time_t timer;
    struct tm* tmlog;
    time(&timer);
    tmlog = localtime(&timer);

    char szTime[64] = {0, };
    sprintf(szTime, "%04d-%02d-%02d %.02d:%.02d:%.02d",
            1900 + tmlog->tm_year, tmlog->tm_mon + 1, tmlog->tm_mday,
            tmlog->tm_hour, tmlog->tm_min, tmlog->tm_sec);

    FILE* pFile = NULL;
    pFile = fopen(m_filepath.c_str(), "a+");
    if (pFile != NULL)
    {
        fprintf(pFile, "%s %s\n", szTime, text);
        fclose(pFile);
    }
}

string MyLogger::GetLogFilePath(const string& logname)
{
	char szTempPath[MAX_PATH] = {0, };
	time_t timer;
	struct tm* tmlog;
	time(&timer);
	tmlog = localtime(&timer);

	DWORD dwTempLength = GetTempPathA(MAX_PATH, szTempPath);

	char szLogPath[MAX_PATH] = {0, };
	sprintf_s(szLogPath, MAX_PATH, "%s\\%s_%d%d%d.log", szTempPath, logname.c_str(), 
			1900 + tmlog->tm_year, tmlog->tm_mon, tmlog->tm_mday);

	string filename = szLogPath;
	return filename;
}

#else
MyLogger::MyLogger(const wstring& wlogname)
{
	m_wfilepath = GetLogFilePathW(wlogname);
	wprintf(L"created.. [%s]\n", m_wfilepath.c_str());
}

void MyLogger::LogW(const wchar_t* fmt, ...)
{
	wchar_t text[MAX_PATH] = {0, };
	va_list args;
	va_start(args, fmt);
	vswprintf_s(text, MAX_PATH, fmt, args);
	va_end(args);

	wprintf(L"%s", text);
	WriteLogW(text);
}

void MyLogger::WriteLogW(const wchar_t* text)
{
	time_t timer;
    struct tm* tmlog;
    time(&timer);
    tmlog = localtime(&timer);

    char wszTime[64] = {0, };
    swprintf(wszTime, L"%04d-%02d-%02d %.02d:%.02d:%.02d",
            1900 + tmlog->tm_year, tmlog->tm_mon + 1, tmlog->tm_mday,
            tmlog->tm_hour, tmlog->tm_min, tmlog->tm_sec);

    FILE* pFile = NULL;
    pFile = _wfopen(m_wfilepath.c_str(), L"a+");
    if (pFile != NULL)
    {
        fwprintf(pFile, L"%s %s\n", wszTime, text);
        fclose(pFile);
    }
}

wstring MyLogger::GetLogFilePathW(const wstring& wlogname)
{
	wchar_t wszTempPath[MAX_PATH] = {0, };
	time_t timer;
	struct tm* tmlog;
	time(&timer);
	tmlog = localtime(&timer);

	DWORD dwTempLength = GetTempPathW(MAX_PATH, wszTempPath);

	wchar_t wszLogPath[MAX_PATH] = {0, };
	swprintf_s(wszLogPath, MAX_PATH, L"%s_%d%d%d.log", wszTempPath, wlogname.c_str(),
			1900 + tmlog->tm_year, tmlog->tm_mon, tmlog->tm_mday);

	wstring wfilename = wszLogPath;
	return wfilename;
}
#endif




//////////////////////////////////////////////////////////////////////////////////////
// main function
//
int main()
{
	// logging
	MyLogger testlogger;
	testlogger.Log("[%s %d %s] %s", __FILE__, __LINE__, __FUNCTION__, "test logger created in main function");
	testlogger.Log("[%s %d %s] %s", __FILE__, __LINE__, __FUNCTION__, "logging module test...");
	testlogger.Log("%s", "data");

	return 0;
}

