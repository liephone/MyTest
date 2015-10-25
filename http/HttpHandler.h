#ifndef _HTTPHANDLER_H
#define _HTTPHANDLER_H

#include <windows.h>
#include <wininet.h>


class HttpHandler
{
public:
	explicit HttpHandler(HINTERNET handle = NULL) : m_handle(handle) {}
	~HttpHandler() { clear(); }

	HINTERNET get() { 
		return m_handle; 
	}
	bool isNull() { 
		return (m_handle == NULL) ? true : false; 
	}
	void clear() { 
		if (m_handle != NULL) { 
			::InternetCloseHandle(m_handle); 
			m_handle = NULL; 
		} 
	}

	void operator = (const HINTERNET& h) { 
		if (m_handle == h) 
			return; 
		clear(); 
		m_handle = h; 
	}

private:
	HINTERNET m_handle;
};

#endif//_HTTPHANDLER_H
