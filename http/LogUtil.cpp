
#include "LogUtil.h"
#include <windows.h>
#include <iostream>
#include <cstdarg>
#include <stdio.h>
#include <time.h>
using namespace std;

/*
 * TODO
 * - thread safe
 * - os independent
 * - exception : buffer overflow - va_list
 */

#define TEXT_BUF_SIZE	8192

#ifndef _UNICODE
LogUtil::LogUtil(const string& logname)
{
	m_filepath = GetLogFilePath(logname);
}

void LogUtil::Log(const char* fmt, ...)
{
	char text[TEXT_BUF_SIZE] = {0, };
	va_list args;
	va_start(args, fmt);
	vsprintf_s(text, TEXT_BUF_SIZE, fmt, args);
	va_end(args);

	WriteLog(text);
}

void LogUtil::WriteLog(const char* text)
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

string LogUtil::GetLogFilePath(const string& logname)
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
LogUtil::LogUtil(const wstring& wlogname)
{
	m_wfilepath = GetLogFilePathW(wlogname);
}

void LogUtil::LogW(const wchar_t* fmt, ...)
{
	wchar_t text[TEXT_BUF_SIZE] = {0, };
	va_list args;
	va_start(args, fmt);
	vswprintf_s(text, TEXT_BUF_SIZE, fmt, args);
	va_end(args);

	WriteLogW(text);
}

void LogUtil::WriteLogW(const wchar_t* text)
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

wstring LogUtil::GetLogFilePathW(const wstring& wlogname)
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

