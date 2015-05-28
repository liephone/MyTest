#include "StdAfx.h"
#include "UrlDownload.h"
#include <afxmt.h>
#include "Util.h"

const static int g_TimeOut = 10000;

CUrlDownload::CUrlDownload(void)
{
}

CUrlDownload::~CUrlDownload(void)
{
}

static UINT urldn(LPVOID param) 
{
	CUrlDownloadBase::UrlDownloadParam *p = (CUrlDownloadBase::UrlDownloadParam *)param;
	CUrlDownload *pPar = static_cast<CUrlDownload*>(p->par);
	try
	{
		p->result = pPar->DownloadReal(p->url,p->dnpath,p->filename,p->statuscode,p->errorcode);
	}
	catch (CMemoryException* e)
	{
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}

	return p->statuscode;
}
bool CUrlDownload::Download(string url,string dnpath,string &filename,DWORD &statuscode,DWORD &errorcode) 
{
	struct CUrlDownloadBase::UrlDownloadParam param;
	param.par		= this;
	param.url		= url;
	param.dnpath	= dnpath;
	param.filename	= filename;
	param.errorcode		= DNE_NOERROR;
	param.statuscode	= 0;
	param.result		= false;

	CWinThread *pThread = NULL;
	pThread = AfxBeginThread(urldn,&param);
	TRACE("\n@@@@CreateThread [%d]",pThread);


	if(pThread == NULL) 
	{
		errorcode = DNE_CREATETHREAD;
		return false;
	}

	while(true) 
	{
		ClearRefresh();
		if(::WaitForSingleObject(pThread->m_hThread,g_TimeOut) == WAIT_TIMEOUT) 
		{
			if(GetRefresh() == false) 
			{
				::TerminateThread(pThread->m_hThread,0);
				pThread		= NULL;
				statuscode	= 0;
				errorcode	= DNE_TIMEOUT;

				TRACE("\n@@@@TreminateThread [%d]",pThread);
				return false;
			}
			else 
			{
				ClearRefresh();
			}
		}
		else 
		{
			pThread		= NULL;
			filename	= param.filename;
			statuscode	= param.statuscode;
			errorcode	= param.errorcode;

			TRACE("\n@@@@TreminateThread [%d]",pThread);
			return param.result;
			
		}
	}
	filename	= param.filename;
	statuscode	= param.statuscode;
	errorcode	= param.errorcode;
	TRACE("\n@@@@TreminateThread [%d]",pThread);
	return param.result;

}
BOOL CUrlDownload::OpenBasic(HINTERNET &hInet,HINTERNET &hConn,HINTERNET &hHttp,string url,DWORD flag)
{
	INTERNET_PORT	nPort		= 0;
	DWORD			dwService	= 0;
	BOOL			bSendReq	= FALSE;
	CString			strServer	= _T("");
	CString			strObject	= _T("");

	::AfxParseURL((LPCTSTR)url.c_str(),dwService,strServer,strObject,nPort);

	hInet		= ::InternetOpen(AGENT_NAME,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);			SetRefresh();
	if(hInet == NULL)	
		return FALSE;

	hConn		= ::InternetConnect(hInet,strServer,nPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);	SetRefresh();
	if(hConn == NULL)	
		return FALSE;


	if(AFX_INET_SERVICE_HTTPS == dwService)
	{
		flag |= (INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	}

	hHttp		= ::HttpOpenRequest(hConn,NULL,strObject,"HTTP/1.0",strServer,NULL,flag,0);		SetRefresh();
	if(hHttp == NULL)	
		return FALSE;

	if(AFX_INET_SERVICE_HTTPS == dwService)
	{
		DWORD dwSecurity = 0;
		dwSecurity = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
			SECURITY_FLAG_IGNORE_REVOCATION |
			SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP |
			SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
			SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
			SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

		::InternetSetOption(hHttp,INTERNET_OPTION_SECURITY_FLAGS,&dwSecurity,sizeof(dwSecurity));
	}

	bSendReq	=  ::HttpSendRequest(hHttp,NULL,0,NULL,0);
	if(!bSendReq){
		//lasterror가  ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED일 때 서버 설정이 SSLClientAuth optional의 다시 시도하면 성공함.
		bSendReq	=  ::HttpSendRequest(hHttp,NULL,0,NULL,0);
	}

	return bSendReq;

}
HINTERNET CUrlDownload::OpenUrl(HINTERNET hInet,string url)
{
	if(hInet == NULL)	
		return NULL;

	HINTERNET hHttp =  ::InternetOpenUrl(hInet,(LPTSTR)url.c_str(),NULL,0,INTERNET_FLAG_RELOAD,0);	SetRefresh();
	return hHttp;
}
bool CUrlDownload::OpenUrlDirect(HINTERNET &hInet, HINTERNET &hHttp,string url)
{
	hInet	= ::InternetOpen(AGENT_NAME,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);				SetRefresh();
	if(hInet == NULL) 
		return false;

	hHttp	= ::InternetOpenUrl(hInet,(LPTSTR)url.c_str(),NULL,0,INTERNET_FLAG_RELOAD,0);	SetRefresh();
	return (hHttp == NULL) ? false : true;
}

bool CUrlDownload::DownloadReal(string url,string dnpath, string &filename,DWORD &statuscode,DWORD &errorcode)
{
	struct tagDownloadNotify Info;

	DWORD dwFlag =	INTERNET_FLAG_DONT_CACHE | 
					//INTERNET_FLAG_RELOAD | 
					INTERNET_FLAG_NO_CACHE_WRITE ;


	HINTERNET hInternet		= NULL;
	HINTERNET hConnect		= NULL;
	HINTERNET hHttpFile		= NULL;

	DWORD dwContentsLen		= 0;


	if(dnpath.at(dnpath.length()-1) != '\\')
		dnpath += _T("\\");

	//CFile file;
	ofstream fout;

	try 
	{
		BOOL SendReqRes = 0;

		filename	= url.substr(url.rfind('/')+1,url.length());
		filename	= Util::Path::MakeValidAxInfoFilename(filename);

		SendReqRes	= OpenBasic(hInternet,hConnect,hHttpFile,url,dwFlag);

		if(SendReqRes != TRUE)
		{
			Close(hConnect,hHttpFile);
			Sleep(500);

			hHttpFile = OpenUrl(hInternet,url);
			
			if(hHttpFile == NULL) 
			{
				Close(hHttpFile,hConnect);
				OpenUrlDirect(hInternet,hHttpFile,url);
				
				if(hHttpFile == NULL)
				{
					Close(hInternet);
					statuscode	= 0;
					errorcode	= DNE_INTERNETOPEN;
					return false;
				}
			}

		}

		statuscode = GetStatusCode(hHttpFile);
		if( statuscode == 407)	//Proxy Auth 실패이면
		{
			Close(hHttpFile,hConnect);
			OpenUrlDirect(hInternet,hHttpFile,url);

			if(hHttpFile == NULL)
			{
				Close(hInternet);
				statuscode	= 0;
				errorcode	= DNE_INTERNETOPEN;
				return false;
			}
		}


		//string attatchfn = GetAttatchement(hHttpFile);
		//if(attatchfn != _T(""))
		//	filename = attatchfn;



		statuscode = GetStatusCode(hHttpFile);
		

		if(statuscode == 200) 
		{
			SetRefresh();
			DWORD dwTotal	= 0;
			DWORD dwReaded	= 0;

			dwContentsLen	= GetContentsLength(hHttpFile);

			Info.type		= DN_START;
			Info.filename	= filename;
			Info.url		= url;
			Info.total		= dwContentsLen;
			Notify(&Info);

			Info.type		= DN_NOTIFY;


			char *pBuff		= new char[URLDOWNLOAD_BUFFSIZE+1];
			string dnfile	= dnpath+filename;

			CUrlDownload::CreateFullPath(dnpath);

			HANDLE hFile = CreateFile(dnfile.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			int err = GetLastError();
			if(hFile != INVALID_HANDLE_VALUE) {
				while(dwContentsLen == 0 || dwTotal < dwContentsLen) 
				{
					SetRefresh();
					memset(pBuff,0,URLDOWNLOAD_BUFFSIZE+1);

					::InternetReadFile(hHttpFile,pBuff,URLDOWNLOAD_BUFFSIZE,&dwReaded);
					if(dwReaded  <= 0)
						break;

					dwTotal += dwReaded;

					DWORD dwWrited;
					WriteFile(hFile,pBuff,dwReaded,&dwWrited,NULL);
					Info.current = dwTotal;
					Notify(&Info);
				}

				CloseHandle(hFile);
			}
			if(pBuff != NULL)
				delete pBuff;
			pBuff = NULL;
		}
		else 
		{
			Close(hInternet,hConnect,hHttpFile);
			errorcode = DNE_STATUSCODE;
			return false;
		}

	}
	catch(CFileException e) 
	{
		Close(hHttpFile,hConnect,hInternet);
		if(fout.is_open() == true)
			fout.close();

		Info.type = DN_ERROR;
		Info.code = -2;
		Notify(&Info);
		errorcode = DNE_FILEWRITE;
		return false;
		}
	catch(...) 
	{
		Close(hHttpFile,hConnect,hInternet);

		if(fout.is_open() == true)
			fout.close();

		Info.type = DN_ERROR;
		Info.code = -1;
		Notify(&Info);
		errorcode = DNE_EXCEPTION;
		return false;
	}

	Info.type = DN_COMPLETE;
	Notify(&Info);

	Close(hHttpFile,hConnect,hInternet);
	errorcode = DNE_NOERROR;
	return true;
}



//진행상황 표시 위함
int CUrlDownload::Notify(struct tagDownloadNotify *pNotify)
{
	return 0;
}

int CUrlDownload::CreateFullPath(string path)
{
	const char dir='\\';

	string::size_type pos = path.find(dir);
	string workpath = path.substr(0,pos);

	CString curpath;
	while(1) 
	{
		pos = path.find(dir,pos+1);
		if(pos == string::npos)
			break;

		workpath = path.substr(0,pos);
		::CreateDirectory(workpath.c_str(),0);
	}
	return 0;
}
