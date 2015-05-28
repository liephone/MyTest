// Mounting a disk or tape
// (Mouting a logical drive to a folder: 논리 드라이브를 폴더로 마운트)
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#define BUFSIZE MAX_PATH
 


// This program run at the command prompt
// This is wide character/unicode version...
int wmain(int argc, wchar_t *argv[])
{
	BOOL bFlag, test;
	// temporary buffer for volume name
	wchar_t Buf[BUFSIZE];
	DWORD cchBufferLength;
	 
	// If not enough arguments supplied...
	// for this example you have to create the mount point first
	if(argc != 3)
	{
		printf("\"%S\" command mounts a volume at a mount point.\n", argv[0]);
		printf("Usage: %S <mount> <volume>\n", argv[0]);
		printf("And make sure the <mount> exists!!!\n");
		printf("Example: \"%S c:\\mymnt\\gdrive\\ g:\\\"\n", argv[0]);

		// Just exit
		return (1);
	}

	wprintf(L"argv[2]: %s\n", argv[2]);
	 
	// Some verification, check the inputs validity
	bFlag = GetVolumeNameForVolumeMountPointW(
				(LPCWSTR)argv[2], // input volume mount point or directory
				(LPWSTR)Buf, // output volume name buffer
				BUFSIZE // size of volume name buffer
				);

	wprintf(L"Buf: %s\n", argv[2]);
	 
	if(bFlag != TRUE)
	{
		printf("Retrieving volume name for %S failed. Error: %d\n", argv[2], GetLastError());
		// Just exit
		return (2);
	}
	printf("Volume name of %S is %S\n", argv[2], Buf);
	 
	bFlag = SetVolumeMountPointW(
				(LPCWSTR)argv[1], // mount point
				(LPCWSTR)Buf // volume to be mounted
				);
	 
	// Another verification, error checking
	if(!bFlag)
		printf("Attempt to mount %S at %S failed.\n", argv[2], argv[1]);
	else
	{
		printf("%S is successfully mounted at %S\n", argv[2], argv[1]);
		cchBufferLength = 100;
		// If the function succeeds, the return value is nonzero.
		// If the function fails, the return value is 0 (zero).
		// To get extended error information, call GetLastError.
		test = GetVolumePathNameW(
					argv[2],
					argv[1],
					cchBufferLength
					);
		 
		if(test != 0)
			printf("The Volume path name for %S is %S.\n", argv[2], argv[1]);
		else
			printf("GetVolumePathName() failed, error %d", GetLastError());
	}

	return (bFlag);
}

