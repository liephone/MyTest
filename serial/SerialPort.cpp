
//#include "stdafx.h"
#include "SerialPort.h"
#include <windows.h>
 

CSerialPort::CSerialPort()
{
}

CSerialPort::~CSerialPort()
{
}

bool CSerialPort::OpenPort(CString portname)
{
	m_hComm = CreateFile(L"//./" + portname, GENERIC_READ | GENERIC_WRITE,
			0, 0, OPEN_EXISTING, 0, 0);
	if (m_hComm == INVALID_HANDLE_VALUE) {
		return false;
	} else {
		return true;
	}
}

bool CSerialPort::ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits)
{
	DCB dcbConfig;
	BOOL bReady = GetCommState(m_hComm, &dcbConfig);
	if (bReady == FALSE) {
		//MessageBox(_T("GetCommState Error"), _T("Error"), MB_OK + MB_ICONERROR);
		CloseHandle(m_hComm);
		return false;
    }
  
	dcbConfig.BaudRate          = BaudRate;  
	dcbConfig.ByteSize          = ByteSize;  
	dcbConfig.Parity            = Parity ;  
	dcbConfig.StopBits          = StopBits;  
	dcbConfig.fBinary           = true;  
	dcbConfig.fDsrSensitivity   = false;  
	dcbConfig.fParity           = fParity;  
	dcbConfig.fOutX             = false;  
	dcbConfig.fInX              = false;  
	dcbConfig.fNull             = false;  
	dcbConfig.fAbortOnError     = true;  
	dcbConfig.fOutxCtsFlow      = false;  
	dcbConfig.fOutxDsrFlow      = false;  
	dcbConfig.fDtrControl       = DTR_CONTROL_DISABLE;  
	dcbConfig.fDsrSensitivity   = false;  
	dcbConfig.fRtsControl       = RTS_CONTROL_DISABLE;  
	dcbConfig.fOutxCtsFlow      = false;  
	dcbConfig.fOutxCtsFlow      = false;  

	bReady = SetCommState(m_hComm, &dcbConfig);
	if (bReady == FALSE) {
		//MessageBox(_T("SetCommState Error"), _T("Error"), MB_OK + MB_ICONERROR);  
		CloseHandle(m_hComm);
		return false;
    }
  
	return true;  
}  
  
bool CSerialPort::SetCommunicationTimeouts(DWORD ReadIntervalTimeout,
		DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant,
		DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant)
{
	COMMTIMEOUTS timeoutConfig;
	BOOL bReady = GetCommTimeouts(m_hComm, &timeoutConfig);
	if (bReady = FALSE) {
		return false;
	}

	timeoutConfig.ReadIntervalTimeout          = ReadIntervalTimeout;
	timeoutConfig.ReadTotalTimeoutConstant     = ReadTotalTimeoutConstant;
	timeoutConfig.ReadTotalTimeoutMultiplier   = ReadTotalTimeoutMultiplier;
	timeoutConfig.WriteTotalTimeoutConstant    = WriteTotalTimeoutConstant;
	timeoutConfig.WriteTotalTimeoutMultiplier  = WriteTotalTimeoutMultiplier;

	bReady = SetCommTimeouts(m_hComm, &timeoutConfig);
	if (bReady == FALSE) {
		//MessageBox(_T("StCommTimeouts function failed"), _T("Com Port Error"), MB_OK | MB_ICONERROR);
		CloseHandle(m_hComm);
		return false;
    }

	return true;
} 
  
bool CSerialPort::WriteByte(BYTE btData)
{
	DWORD dwBytesWritten = 0;
	if (WriteFile(m_hComm, &btData, 1, &dwBytesWritten, NULL) == 0) {
        return false;
	} else {
		return true;  
	}
}

bool CSerialPort::ReadByte(BYTE &resp)
{  
	BYTE rx;  
    resp = 0;
	DWORD dwBytesTransferred = 0;

	if (ReadFile(m_hComm, &rx, 1, &dwBytesTransferred, 0)) {
		if (dwBytesTransferred == 1) {
			resp = rx;
			return true;
		}
	}

	return false;
}

bool CSerialPort::ReadByte(BYTE* &resp, UINT size)
{  
	DWORD dwBytesTransferred = 0;

	if (ReadFile(m_hComm, resp, size, &dwBytesTransferred, 0)) {
		if(dwBytesTransferred == size) {
            return true;
		}
    }

	return false;
}
  
void CSerialPort::ClosePort()
{
	CloseHandle(m_hComm);
	return;
}

