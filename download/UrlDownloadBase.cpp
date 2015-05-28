#include <StdAfx.h>
#include "UrlDownloadBase.h"

int CUrlDownloadBase::GetStatusCode(HINTERNET hHttpFile)
{
	DWORD dwCode = 0;
	DWORD szCode = sizeof(dwCode);
	::HttpQueryInfo(hHttpFile,HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER,&dwCode,&szCode,NULL);
	return dwCode;
}

int CUrlDownloadBase::GetContentsLength(HINTERNET hHttpFile)
{
	DWORD dwLen = 0;
	DWORD szLen = sizeof(dwLen);
	::HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,&dwLen,&szLen,NULL);
	return dwLen;
}
string CUrlDownloadBase::GetAttatchement(HINTERNET hHttpFile)
{
	char buff[512] = {0,};
	DWORD bufflen = sizeof(buff);
	::HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_DISPOSITION,buff,&bufflen,NULL);
	string info(buff);
	string filename = _T("filename=");
	string::size_type fnpos = info.find(filename,0);
	if(fnpos == string::npos)
		return _T("");

	string::size_type endpos = info.find(";",fnpos);
	if(endpos == string::npos)
		endpos = info.length();


	fnpos += filename.length();
	info = info.substr(fnpos,endpos-fnpos);
	return info;
}