#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
//#include "SerialPort.h"
#include "A7Command.h"
using namespace std;


int main()
{
	vector<string> ports;

	for (int i = 1; i <= 255; i++) {
		char szPortName[32] = {0, };
		sprintf(szPortName, "\\\\.\\COM%d", i);
		HANDLE hSerial = CreateFile(szPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
		if (hSerial == INVALID_HANDLE_VALUE)
			continue;

		string port = szPortName;
		ports.push_back(port);
	}

	printf("Available COM ports\n");
	for (int i = 0; i < ports.size(); i++) {
		printf("%s\n", ports.at(i).c_str());
	}

	printf("cmd_HT[%s], [%d][%d]\n", cmd_HT, sizeof(cmd_HT), strlen((const char*)cmd_HT));

	/*
	CSerial serial;
	long lRet = serial.Open(strPort);
	lRet = serial.Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
    lRet = serial.SetupHandshaking(CSerial::EHandshakeOff);

	lRet = serial.Close();
	*/

	return 0;
}
