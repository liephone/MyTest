#ifndef _WININET_H
#define _WININET_H

#include <windows.h>
#include <wininet.h>		// Wininet.lib
#include <string>
#include <map>
#include "HttpHandler.h"

#pragma comment(lib, "Wininet.lib")

class LogUtil;

class WinINet 
{
public:
	explicit WinINet(HINTERNET handle = NULL);
	~WinINet();

	std::string Get(const std::string& url);
	std::string Post(const std::string& url, const std::map<std::string, std::string>& data);

private:
	std::string post(const std::string& url, const std::string& data);

	bool open();
	int getStatusCode(HINTERNET handle);
	std::string readData(HINTERNET handle);
	void parseUrl(const std::string& url, std::string &protocol, std::string &server, std::string &path, std::string &param, int &port);

private:
	HttpHandler m_session;
	LogUtil* m_logger;
};


#endif//_WININET_H
