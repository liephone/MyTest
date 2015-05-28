#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "Advapi32.lib")

int SH_StartService(LPCTSTR szServiceName);
DWORD SH_CheckServiceStatus(LPCTSTR szServiceName);
void TestStart();

int main()
{
	TCHAR tszServiceName[512] = _T("safehardmounter");

	SC_HANDLE schSCManager = OpenSCManager( 
        NULL,
        NULL,
        SC_MANAGER_ENUMERATE_SERVICE);
    if (NULL == schSCManager) {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return -1;
    }

    SC_HANDLE schService = OpenService( 
        schSCManager,
        tszServiceName,
        SC_MANAGER_ENUMERATE_SERVICE);
 
    if (schService == NULL) { 
        printf("OpenService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return -1;
    }    

	SERVICE_STATUS_PROCESS ssStatus;
	DWORD dwBytesNeeded;
    if (!QueryServiceStatusEx( 
            schService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE) &ssStatus,
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded))
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(schService); 
        CloseServiceHandle(schSCManager);
        return -1; 
    }

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);

    if (ssStatus.dwCurrentState == SERVICE_STOPPED || ssStatus.dwCurrentState == SERVICE_STOP_PENDING) {
		printf("stopped service\n");
		SH_StartService(tszServiceName);
    } else {
        printf("Cannot start the service because it is already running\n");
        return -1; 

		/*
		if (StartService(schService, 0, NULL)) {
			printf("Service started\n");
		} else {
			printf("StartService failed (%d)\n", GetLastError()); 
		}
		*/
	}

	return 0;
}


void TestStart()
{
}


BOOL SH_StartService(LPCTSTR ServiceName)
{
	SC_HANDLE controlHandle;
	SC_HANDLE serviceHandle;
	SERVICE_STATUS ss;
	BOOL result = TRUE;

	_tprintf(_T("SH_StartService(%s)\n"), ServiceName);

	controlHandle = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (controlHandle == NULL) {
		printf("failed to open SCM: %d\n", GetLastError());
		return FALSE;
	}

	serviceHandle = OpenService(controlHandle, ServiceName,
		SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
	if (serviceHandle == NULL) {
		_tprintf(_T("failed to open Service (%s): %d\n)"), ServiceName, GetLastError());
		CloseServiceHandle(controlHandle);
		return FALSE;
	}
	
	if (StartService(serviceHandle, 0, NULL)) {
		_tprintf(_T("Service (%s) started\n"), ServiceName);
		result = TRUE;
	} else {
		_tprintf(_T("failed to start service (%s): %d\n"), ServiceName, GetLastError());
		result = FALSE;
	}

	return result;
}


DWORD SH_CheckServiceStatus(LPCTSTR szServiceName)
{
	DWORD dwResult = 0;


	return dwResult;
}

