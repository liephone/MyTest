
#include "WinINetClient.h"
#include "LogUtil.h"
#include "URLCodec.h"
//#include <CommonException.h>

#define CLIENT_NAME		"MobilePosBell Client/1.0"
#define READ_BUFFSIZE	(5 * 1024)

using namespace std;


WinINet::WinINet(HINTERNET handle)
	: m_handle(handle),
	  m_logger(NULL)
{
	m_logger = new LogUtil("wininetclient");
	m_logger->Log("[WinINet] created..");
}

WinINet::~WinINet()
{
	m_logger->Log("[WinINet] destroy..");
	close();
	if (m_logger != NULL) {
		delete m_logger;
		m_logger = NULL;
	}
}

string WinINet::Get(const string& url)
{
	if (m_handle == NULL) {
		m_logger->Log("[WinINet] GET::handle is NULL !!!");
		open();
	}

	int port = 0;
	string protocol, server, path, param;
	parseUrl(url, protocol, server, path, param, port);

	m_handle = ::InternetConnectA(m_handle, server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (m_handle == NULL) {
		//throw NetworkException(ErrorCode::SH_C_CONNECTION,GetLastError());
		m_logger->Log("[WinINet][ERROR] GET::InternetConnect() failed !!!]");
	} else {
		m_logger->Log("[WinINet] GET::InternetConnect() ok.");
	}

	DWORD flag = INTERNET_FLAG_DONT_CACHE |
				//INTERNET_FLAG_RELOAD |
				INTERNET_FLAG_NO_CACHE_WRITE;
	if (protocol.find("https") != string::npos) {
		flag |= (INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	}

	m_handle = ::HttpOpenRequestA(m_handle, NULL, path.c_str(), "HTTP/1.0", server.c_str(), NULL, flag, 0);
	if (m_handle == NULL) {
		//throw NetworkException(ErrorCode::SH_C_REQUEST,0,string(""),string(""));
		m_logger->Log("[WinINet][ERROR] GET::HttpOpenRequest() failed !!!)");
	} else {
		m_logger->Log("[WinINet] GET::HttpOpenRequest() ok.");
	}

	if (protocol.find("https") != string::npos) {
		DWORD dwSecurity = 0;
		dwSecurity = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
			SECURITY_FLAG_IGNORE_REVOCATION |
			SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP |
			SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
			SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
			SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

		::InternetSetOptionA(m_handle, INTERNET_OPTION_SECURITY_FLAGS, &dwSecurity, sizeof(dwSecurity));
	}

	BOOL bSendReq = FALSE;
	bSendReq = ::HttpSendRequestA(m_handle, NULL, 0, NULL, 0);
	if (!bSendReq) {
		bSendReq = ::HttpSendRequestA(m_handle, NULL, 0, NULL, 0);
	}
	if (!bSendReq) {
		//throw NetworkException(ErrorCode::SH_C_REQUEST,GetLastError(),string(""),string(""));
		m_logger->Log("[WinINet][ERROR] GET::HttpSendRequest() failed !!!");	// error code
	}

	m_logger->Log("[WinINet] GET::HttpSendRequest() ok.");
	

	int statuscode = getStatusCode();
	if (statuscode != HTTP_STATUS_OK) {
		//debug() << "WinHttp::get : Code " << statuscode;
		string res = readData();
		//debug() << "WinHttp::get : Receive " << res;
		//throw NetworkException(ErrorCode::SH_C_HTTPOK,statuscode,string(""),string(""));
		m_logger->Log("[WinINet][ERROR] GET::HTTP status failed !!!");
	}

	m_logger->Log("[WinINet] GET::status code [%d]", statuscode);
	string res = readData();
	m_logger->Log("[WinINet] GET::read data [%d]\n[%s]", res.size(), res.c_str());
	close();

	return res;
}

string WinINet::Post(const string& url, map<string, string>& data)
{
	string strData = URLCodec::encodeFromMap(mapData);
	string res = post(url, strData);
	return res;
}

string WinINet::post(const string& url, const string& data)
{
	if (m_handle == NULL) {
		m_logger->Log("[WinINet] POST::handle is NULL !!!");
		open();
	}

	int port;
	string protocol, server, path, param;
	parseUrl(url, protocol, server, path, param, port);

	m_handle = ::InternetConnectA(m_handle, server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (m_handle == NULL) {
		//throw NetworkException(ErrorCode::SH_C_CONNECTION,GetLastError());
		m_logger->Log("[WinINet][ERROR] POST::InternetConnect() failed !!!");
	} else {
		m_logger->Log("[WinINet] POST::InternetConnect() ok.");
	}

	DWORD flag = INTERNET_FLAG_DONT_CACHE |
				//INTERNET_FLAG_RELOAD |
				INTERNET_FLAG_NO_CACHE_WRITE;

	if (protocol.find("https") != string::npos) {
		flag |= (INTERNET_FLAG_SECURE | 
				INTERNET_FLAG_IGNORE_CERT_CN_INVALID | 
				INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	}

	m_handle = ::HttpOpenRequestA(m_handle, "POST", path.c_str(), "HTTP/1.0", server.c_str(), NULL, flag, 0);
	if (m_handle == NULL) {
		//throw NetworkException(ErrorCode::SH_C_REQUEST,0,string(""),string(""));
		m_logger->Log("[WinINet][ERROR] POST::HttpOpenRequest() failed !!!");
	} else {
		m_logger->Log("[WinINet] POST::HttpOpenRequest() ok.");
	}

	if (protocol.find("https") != string::npos) {
		DWORD dwSecurity = 0;
		dwSecurity = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
					SECURITY_FLAG_IGNORE_REVOCATION |
					SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP |
					SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
					SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
					SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

		::InternetSetOptionA(m_handle, INTERNET_OPTION_SECURITY_FLAGS, &dwSecurity, sizeof(dwSecurity));
	}

	string header = ("Content-Type: application/x-www-form-urlencoded");
	BOOL bSendReq = FALSE;
	bSendReq = ::HttpSendRequestA(m_handle, header.c_str(), header.size(), (LPVOID)data.c_str(), (DWORD)data.size());
	if (!bSendReq){
		bSendReq = ::HttpSendRequestA(m_handle, header.c_str(), header.size(), (LPVOID)data.c_str(), (DWORD)data.size());
	}
	if (!bSendReq) {
		//throw NetworkException(ErrorCode::SH_C_REQUEST,GetLastError(),string(""),string(""));
		m_logger->Log("[WinINet][ERROR] POST::HttpSendRequest() failed !!!");
	}
	m_logger->Log("[WinINet] POST::HttpSendRequest() ok.");

	int statuscode = getStatusCode();
	if (statuscode != HTTP_STATUS_OK) {
		//debug() << "WinHttp::get : Code " << statuscode;
		string res = readData(hHttp.get());
		//debug() << "WinHttp::get : Receive " << res;
		//throw NetworkException(ErrorCode::SH_C_HTTPOK,statuscode,string(""),string(""));
		m_logger->Log("[WinINet][ERROR] POST::HTTP status failed !!!");
	}

	m_logger->Log("[WinINet] POST::status code [%d]", statuscode);
	string res = readData();
	m_logger->Log("[WinINet] POST::read data [%d]\n[%s]", res.size(), res.c_str());
	return res;
}

bool WinINet::open()
{
	m_handle = ::InternetOpenA(CLIENT_NAME, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (m_handle == NULL) {
		// throw NetworkException(..)
		m_logger->Log("open : Exception throw [InternetOpen fail !!!]");
		return false;
	} else {
		m_logger->Log("open : InternetOpen() ok.");
		
		return true;
	}
}

void WinINet::close()
{
	if (m_handle != NULL) {
		::InternetCloseHandle(m_handle);
		m_logger->Log("close : InternetCloseHandle() ok.\n");
		m_handle = NULL;
	}
}

int WinINet::getStatusCode()
{
	DWORD dwCode = 0;
	DWORD szCode = sizeof(dwCode);
	::HttpQueryInfoA(m_handle, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &szCode, NULL);
	return dwCode;
}

string WinINet::readData()
{
	DWORD dwSize = 0;
	DWORD dwReaded = 0;
	char* pBuff = new char[READ_BUFFSIZE + 1];
	string res;

	do {
		dwReaded = 0;
		memset(pBuff, 0, READ_BUFFSIZE + 1);
		::InternetReadFile(m_handle, pBuff, READ_BUFFSIZE, &dwReaded);
		if (dwReaded <= 0) {
			break;
		}

		res += string(pBuff);
	} while (true);

	if (pBuff != NULL) {
		delete pBuff;
	}

	return res;
}

void WinINet::parseUrl(const string& url, string &protocol, string &server, string &path, string &param, int &port)
{
	URL_COMPONENTSA urlComponents;
	memset(&urlComponents, 0, sizeof(URL_COMPONENTS));

	urlComponents.dwStructSize = sizeof(URL_COMPONENTS);
	urlComponents.dwSchemeLength = INTERNET_MAX_SCHEME_LENGTH;
	urlComponents.lpszScheme = new char[INTERNET_MAX_SCHEME_LENGTH + 1];
	urlComponents.dwHostNameLength = INTERNET_MAX_HOST_NAME_LENGTH;
	urlComponents.lpszHostName = new char[INTERNET_MAX_HOST_NAME_LENGTH + 1];
	urlComponents.dwUrlPathLength = INTERNET_MAX_PATH_LENGTH;
	urlComponents.lpszUrlPath = new char[INTERNET_MAX_PATH_LENGTH + 1];
	urlComponents.dwUserNameLength = INTERNET_MAX_USER_NAME_LENGTH;
	urlComponents.lpszUserName = new char[INTERNET_MAX_USER_NAME_LENGTH + 1];
	urlComponents.dwPasswordLength = INTERNET_MAX_PASSWORD_LENGTH;
	urlComponents.lpszPassword = new char[INTERNET_MAX_PASSWORD_LENGTH + 1];
	urlComponents.dwExtraInfoLength = INTERNET_MAX_PATH_LENGTH;
	urlComponents.lpszExtraInfo = new char[INTERNET_MAX_PATH_LENGTH + 1];

	InternetCrackUrlA(url.c_str(), 0, 0, &urlComponents);

	protocol = urlComponents.lpszScheme;
	server = urlComponents.lpszHostName;
	path = urlComponents.lpszUrlPath;
	param = urlComponents.lpszExtraInfo;
	port = urlComponents.nPort;

	delete urlComponents.lpszScheme;
	delete urlComponents.lpszHostName;
	delete urlComponents.lpszUrlPath;
	delete urlComponents.lpszUserName;
	delete urlComponents.lpszPassword;
	delete urlComponents.lpszExtraInfo;
}

