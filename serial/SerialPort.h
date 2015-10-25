#ifndef _SERIALPORT_H
#define _SERIALPORT_H



class CSerialPort  
{  
public:  
	CSerialPort(void);  
	virtual ~CSerialPort(void);  

public:  
	bool OpenPort(CString portname);  
	void ClosePort();  
	bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier,DWORD WriteTotalTimeoutConstant);  
	bool ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits);  
	bool ReadByte(BYTE &resp);  
	bool ReadByte(BYTE* &resp, UINT size);  
	bool WriteByte(BYTE btData);  

private:  
	HANDLE m_hComm;
};

#endif//_SERIALPORT

