// Psapi.lib
//
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>

int main()
{
	
	// get process name by pid
	int pid = 3948;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
			FALSE, 
			pid
	);
	if (hProcess) {
		TCHAR buff[MAX_PATH] = {0, };
		if (GetModuleFileNameEx(hProcess, 0, buff, MAX_PATH)) {
			printf("%d: %s\n", pid, buff);
		} else {
			printf("not found pid.\n");

		}

		if (GetModuleBaseName(hProcess, 0, buff, MAX_PATH)) {
			printf("%d: %s\n", pid, buff);
		} else {
			printf("not found pid.\n");
		}

		if (GetProcessImageFileName(hProcess, buff, MAX_PATH)) {
			printf("%d: %s\n", pid, buff);
		} else {
			printf("not found pid.\n");
		}

		DWORD dwSize = 0;
		if (QueryFullProcessImageName(hProcess, 0, buff, &dwSize)) {
			printf("%d: %s\n", pid, buff);
		} else {
			printf("not found pid.\n");
		}

		CloseHandle(hProcess);
	}
	

	/*
	// enumerate processes
	DWORD process_id_array[1024] = {0, };
	DWORD bytes_returned = 0;
	DWORD num_processes = 0;
	HANDLE hProcess = NULL;

	if (EnumProcesses(process_id_array, 256 * sizeof(DWORD), &bytes_returned)) {
		num_processes = (bytes_returned / sizeof(DWORD));
		for (int i = 0; i < num_processes; i++) {
			printf("%d\n", process_id_array[i]);
		}
	} else {
		printf("EnumProcesses failed !!\n");
	}
	*/

	return 0;
}

