#include <windows.h>
#include <tchar.h>
#include <stdio.h>


int main()
{
	// dokan device driver (service) 와 연결 테스트 

	// Mount Point(target device) 생성
	// - CreateFile()
	// - DeviceIoControl()
	
	// Drive Letter 할당
	// - GetVolumeNameForVolumeMountPoint()
	// - DefineDosDevice()
	// - SetVolumeMountPoint()


	// (DeleteVolumeMountPoint)
	// DefineDosDevice
	// GetVolumeNameForVolumeMountPoint
	// (DefineDosDevice : 삭제)
	// SetVolumeMountPoint

	/* 1. 디바이스 정보 로드 or 생성 */
	// - 대상 디바이스 오픈 (CreateFile: \\.\Dokan, open)
	TCHAR tszDevice[] = _T("\\\\.\\Dokan");
	HANDLE hDevice = NULL;
	hDevice = CreateFile(
					tszDevice, 
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					0,
					NULL
			);
	if (hDevice == INVALID_HANDLE_VALUE) {
		_tprintf(_T("invalid handle : [%s]\n"), tszDevice);
	}

	// - 디바이스 볼륨 생성, 이벤트 시작 및 정보 리턴 (DeviceIoControl: HANDLE	device, IOCTL_EVENT_START)
	//   -> sys::event::DokanEventStart
	//   DeviceName 0x0442f3dc L"\\Volume{d6cc17c5-1734-4085-bce7-964f1e9f5de9}"	wchar_t[64]
	//   #define DOKAN_BASE_GUID  {0xd6cc17c5, 0x1734, 0x4085, {0xbc, 0xe7, 0x96, 0x4f, 0x1e, 0x9f, 0x5d, 0xe9}}
	//                     _GUID   Data1       Data2   Data3   Data4


	/* 2. 디바이스 target 정의 : Define Dos Device */
	TCHAR tszDeviceName[] = _T("O:");
	TCHAR tszTargetPath[] = _T("\\Device\\Dokan");

	BOOL bResult = DefineDosDevice(DDD_RAW_TARGET_PATH, tszDeviceName, tszTargetPath);
	if (bResult) {
		_tprintf(_T("DefineDosDevice success: %s, %s\n"), tszDeviceName, tszTargetPath);
	} else {
		_tprintf(_T("DefineDosDevice failed: error[%d]\n"), GetLastError());
	}
	
	/* - 확인 Query Dos Device */
	TCHAR tszQueryPath[MAX_PATH] = {0, };
	
	DWORD dwResult = QueryDosDevice(_T("O:"), tszQueryPath, MAX_PATH);
	if (dwResult > 0) {
		_tprintf(_T("QueryDosDevice success: %s\n"), tszQueryPath);
	} else {
		_tprintf(_T("QueryDosDevice failed: error[%d]\n"), GetLastError());
	}

	/* - Get volume GUID path */
	TCHAR tszVolumeGUID[MAX_PATH] = _T("\\Device\\Volume{d6cc17c5-1734-4085-bce7-964f1e9f5de9}");
	TCHAR tszMountPoint[] = _T("O:\\");

	/*
	bResult = GetVolumeNameForVolumeMountPoint(tszMountPoint, (LPTSTR)tszVolumeGUID, MAX_PATH);
	if (bResult) {
		_tprintf(_T("volume GUID: %s\n"), tszVolumeGUID);
	} else {
		_tprintf(_T("failed: error[%d]\n"), GetLastError());
	}
	*/


	/* 3. 볼륨 드라이브 연결 : SetVolumeMountPoint 로 가능?? */
	bResult = SetVolumeMountPoint(tszMountPoint, tszVolumeGUID);
	if (bResult) {
		_tprintf(_T("SetVolumeMountPoint success\n"));
	} else {
		_tprintf(_T("SetVolumeMountPoint failed: error[%d]\n"), GetLastError());
	}


	return 0;
}

