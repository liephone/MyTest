
#include "WinINetClient.h"
#include "LogUtil.h"
#include "URLCodec.h"
//#include <CommonException.h>

#define CLIENT_NAME		"MobilePosBell Client/1.0"
#define READ_BUFFSIZE	(5 * 1024)

using namespace std;


WinINet::WinINet(HINTERNET handle)
	: m_session(handle),
	  m_logger(NULL)
{
	m_logger = new LogUtil("wininetclient");
	m_logger->Log("\n[WinINet] created..");
}

WinINet::~WinINet()
{
	m_logger->Log("[WinINet] destroy..");
	if (m_logger != NULL) {
		delete m_logger;
		m_logger = NULL;
	}
}

string WinINet::Get(const string& url)
{
	m_logger->Log("[WinINet] HTTP GET : <%s>", url.c_str());

	if (m_session.isNull()) {
		m_logger->Log("[WinINet] GET::session is NULL !!!");
		open();
	}

	int port = 0;
	string protocol, server, path, param;
	parseUrl(url, protocol, server, path, param, port);
	path += param;

	//m_logger->Log("[WinINet] GET::parseUrl : protocol[%s] server[%s] path[%s] param[%s] port[%d]", protocol.c_str(), server.c_str(), path.c_str(), param.c_str(), port);

	HttpHandler hConnect;
	hConnect = ::InternetConnectA(m_session.get(), server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hConnect.isNull()) {
		m_logger->Log("[WinINet][ERROR] GET::InternetConnect() failed !!!]");
		//throw NetworkException(ErrorCode::SH_C_CONNECTION,GetLastError());
	} else {
		m_logger->Log("[WinINet] GET::InternetConnect() ok.");
	}

	DWORD flag = INTERNET_FLAG_DONT_CACHE |
				//INTERNET_FLAG_RELOAD |
				INTERNET_FLAG_NO_CACHE_WRITE;
	if (protocol.find("https") != string::npos) {
		flag |= (INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	}

	HttpHandler hRequest;
	hRequest = ::HttpOpenRequestA(hConnect.get(), NULL, path.c_str(), "HTTP/1.1", server.c_str(), NULL, flag, 0);
	if (hRequest.isNull()) {
		m_logger->Log("[WinINet][ERROR] GET::HttpOpenRequest() failed !!!)");
		//throw NetworkException(ErrorCode::SH_C_REQUEST,0,string(""),string(""));
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

		::InternetSetOptionA(hRequest.get(), INTERNET_OPTION_SECURITY_FLAGS, &dwSecurity, sizeof(dwSecurity));
	}

	BOOL bSendReq = FALSE;
	bSendReq = ::HttpSendRequestA(hRequest.get(), NULL, 0, NULL, 0);
	if (!bSendReq) {
		bSendReq = ::HttpSendRequestA(hRequest.get(), NULL, 0, NULL, 0);
	}
	if (!bSendReq) {
		m_logger->Log("[WinINet][ERROR] GET::HttpSendRequest() failed !!!");	// error code
		//throw NetworkException(ErrorCode::SH_C_REQUEST,GetLastError(),string(""),string(""));
	}

	m_logger->Log("[WinINet] GET::HttpSendRequest() ok.");
	

	int statuscode = getStatusCode(hRequest.get());
	if (statuscode != HTTP_STATUS_OK) {
		string fail_res = readData(hRequest.get());
		m_logger->Log("[WinINet][ERROR] GET::read data [%d]\n[%s]", fail_res.size(), fail_res.c_str());
		m_logger->Log("[WinINet][ERROR] GET::HTTP status failed !!!");
		//throw NetworkException(ErrorCode::SH_C_HTTPOK,statuscode,string(""),string(""));
	}

	m_logger->Log("[WinINet] GET::status code [%d]", statuscode);
	string res = readData(hRequest.get());
	m_logger->Log("[WinINet] GET::read data [%d]\n[%s]", res.size(), res.c_str());

	return res;
}

string WinINet::Post(const string& url, const map<string, string>& data)
{
	m_logger->Log("[WinINet] HTTP POST : <%s>", url.c_str());
	
	string strData = URLCodec::encodeFromMap(data);
	string res = post(url, strData);
	return res;
}

string WinINet::post(const string& url, const string& data)
{
	if (m_session.isNull()) {
		m_logger->Log("[WinINet] POST::handle is NULL !!!");
		open();
	}

	int port;
	string protocol, server, path, param;
	parseUrl(url, protocol, server, path, param, port);

	//m_logger->Log("[WinINet] POST::parseUrl : protocol[%s] server[%s] path[%s] param[%s] port[%d]", protocol.c_str(), server.c_str(), path.c_str(), param.c_str(), port);

	HttpHandler hConnect(::InternetConnectA(m_session.get(), server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0));
	if (hConnect.isNull()) {
		m_logger->Log("[WinINet][ERROR] POST::InternetConnect() failed !!!");
		//throw NetworkException(ErrorCode::SH_C_CONNECTION,GetLastError());
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

	HttpHandler hRequest(::HttpOpenRequestA(hConnect.get(), "POST", path.c_str(), "HTTP/1.1", server.c_str(), NULL, flag, 0));
	if (hRequest.isNull()) {
		m_logger->Log("[WinINet][ERROR] POST::HttpOpenRequest() failed !!!");
		//throw NetworkException(ErrorCode::SH_C_REQUEST,0,string(""),string(""));
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

		::InternetSetOptionA(hRequest.get(), INTERNET_OPTION_SECURITY_FLAGS, &dwSecurity, sizeof(dwSecurity));
	}

	string header = ("Content-Type: application/x-www-form-urlencoded");
	BOOL bSendReq = FALSE;
	bSendReq = ::HttpSendRequestA(hRequest.get(), header.c_str(), header.size(), (LPVOID)data.c_str(), (DWORD)data.size());
	if (!bSendReq){
		bSendReq = ::HttpSendRequestA(hRequest.get(), header.c_str(), header.size(), (LPVOID)data.c_str(), (DWORD)data.size());
	}
	if (!bSendReq) {
		m_logger->Log("[WinINet][ERROR] POST::HttpSendRequest() failed !!!");
		//throw NetworkException(ErrorCode::SH_C_REQUEST,GetLastError(),string(""),string(""));
	}
	m_logger->Log("[WinINet] POST::HttpSendRequest() ok.");

	int statuscode = getStatusCode(hRequest.get());
	if (statuscode != HTTP_STATUS_OK) {
		m_logger->Log("[WinINet][ERROR] POST::HTTP status failed !!!");
		string fail_res = readData(hRequest.get());
		m_logger->Log("[WinINet][ERROR] POST::read data [%d]\n[%s]", fail_res.size(), fail_res.c_str());
		//throw NetworkException(ErrorCode::SH_C_HTTPOK,statuscode,string(""),string(""));
	}

	m_logger->Log("[WinINet] POST::status code [%d]", statuscode);
	string res = readData(hRequest.get());
	m_logger->Log("[WinINet] POST::read data [%d]\n[%s]", res.size(), res.c_str());
	return res;
}

bool WinINet::open()
{
	m_session = ::InternetOpenA(CLIENT_NAME, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (m_session.isNull()) {
		m_logger->Log("[WinINet][ERROR] Open::InternetOpen() fail !!!]");
		// throw NetworkException(..)
		return false;
	} else {
		m_logger->Log("[WinINet] Open::InternetOpen() ok.");
		
		return true;
	}
}

int WinINet::getStatusCode(HINTERNET handle)
{
	DWORD dwCode = 0;
	DWORD szCode = sizeof(dwCode);
	::HttpQueryInfoA(handle, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &szCode, NULL);
	return dwCode;
}

string WinINet::readData(HINTERNET handle)
{
	DWORD dwSize = 0;
	DWORD dwReaded = 0;
	char* pBuff = new char[READ_BUFFSIZE + 1];
	string res;

	do {
		dwReaded = 0;
		memset(pBuff, 0, READ_BUFFSIZE + 1);
		::InternetReadFile(handle, pBuff, READ_BUFFSIZE, &dwReaded);

		m_logger->Log("[WinINet] ReadData::InternetReadFile() : deRead[%d]", dwReaded);		

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

