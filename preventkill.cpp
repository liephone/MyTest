#include <windows.h>
#include <process.h>
#include <Aclapi.h>
#include <stdio.h>

#pragma comment(lib, "Advapi32.lib")

DWORD ProtectProcess(void)
{
    HANDLE hProcess = GetCurrentProcess();
    PACL pEmptyDacl;
    DWORD dwErr;

    // using malloc guarantees proper alignment
    pEmptyDacl = (PACL)malloc(sizeof(ACL));

    if (!InitializeAcl(pEmptyDacl, sizeof(ACL), ACL_REVISION)) {
        dwErr = GetLastError();
    } else {
        dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT, 
                   DACL_SECURITY_INFORMATION, NULL, NULL, pEmptyDacl, NULL);
    }

    free(pEmptyDacl);
    return dwErr;
}

int main()
{
	ProtectProcess();

	while (1) {
		Sleep(5000);
		printf("alive...\n");
	}

	return 0;
}

