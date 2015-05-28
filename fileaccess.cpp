#include <afx.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <windows.h>
#include <shellapi.h>
//#include "common/util.h"
//#include "common/ComUtil.h"
#include <IEPMapi.h>
#include <Shlobj.h>
#include <commdlg.h>
#include <sddl.h>
#include <AccCtrl.h>
#include <AclAPI.h>
using namespace std;

#ifdef _UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif


bool TestCFile(tstring filename);
bool TestIOStream(tstring filename);
bool TestCreateFile(tstring filename);
bool TestFOpen(tstring filename);
bool DeleteFile(tstring filename);
void PrintMessage(LPCTSTR msg);
HANDLE LaunchViaCreateProcess(LPCTSTR program, LPCTSTR args);
HANDLE LaunchViaShellExecute(LPCTSTR program, LPCTSTR arg);
bool LaunchViaExecuteBlock(LPCTSTR program, LPCTSTR arg);
HRESULT SaveFileData(LPWSTR pszName, BYTE* pbData, UINT cbBytes, LPWSTR* ppszDest);
HRESULT SaveFileProtected(LPWSTR pszName, LPWSTR pszSource, LPWSTR *ppszDest);
void CreateLowProcess();
void SetLowLabelToFile();
void ShowProcessIntegrityLevel();

int main() 
{
	// test path
	vector<tstring> testpath;
	testpath.push_back(_T("C:\\"));
	/*
	testpath.push_back(_T("C:\\NPKI\\"));
	testpath.push_back(_T("C:\\Program Files\\"));
	testpath.push_back(_T("C:\\Program Files (x86)\\"));
	testpath.push_back(_T("C:\\Program Files (x86)\\Wizvera\\Veraport20\\"));
	testpath.push_back(_T("C:\\Users\\"));
	testpath.push_back(_T("C:\\Users\\kskim\\"));
	testpath.push_back(_T("C:\\Users\\kskim\\AppData\\Local\\")); 
	testpath.push_back(_T("C:\\Users\\kskim\\AppData\\Local\\Temp\\")); 
	testpath.push_back(_T("C:\\Users\\kskim\\AppData\\LocalLow\\"));
	testpath.push_back(_T("C:\\Users\\kskim\\AppData\\Local\\Packages\\windows_ie_ac_001\\AC\\"));
	testpath.push_back(_T("C:\\Users\\kskim\\AppData\\Local\\Packages\\windows_ie_ac_001\\AC\\Temp\\"));
	testpath.push_back(_T("C:\\Users\\kskim\\AppData\\Roaming\\"));
	testpath.push_back(_T("C:\\Windows\\"));
	testpath.push_back(_T("C:\\Windows\\System32\\"));
	testpath.push_back(_T("C:\\Windows\\SysWOW64\\"));
	testpath.push_back(_T("I:\\NPKI\\"));
	testpath.push_back(_T("K:\\NPKI\\"));
	*/
	
	tstring filename = _T("test.txt");
	

	// file access test
	for (int i = 0; i < testpath.size(); i++) 
	{
		//TestCFile(testpath.at(i) + filename);
		TestIOStream(testpath.at(i) + filename);
		//TestCreateFile(testpath.at(i) + filename);
		//TestFOpen(testpath.at(i) + filename);
	}

	// delete
	for (int i = 0; i < testpath.size(); i++)
	{
		//DeleteFile(testpath.at(i) + filename);
	}

	/*
	// saving file in Protected Mode
	BYTE pbData[512] = "internet explorer protected mode process test : file save ...";
	wchar_t pszDest[MAX_PATH] = {0, };
	SaveFileData(L"test_", pbData, sizeof(pbData), (LPWSTR*)&pszDest);
	::MessageBoxW(NULL, pszDest, L"test", MB_OK);

	//AfxMessageBox(_T("FinalConstruct() :: process test"));

	// process test 
	tstring program1 = _T("C:\\Users\\wiz\\AppData\\Local\\Packages\\windows_ie_ac_001\\AC\\delfino.exe");
	tstring program2 = _T("C:\\Windows\\System32\\notepad.exe");
	tstring program3 = _T("C:\\Program Files\\Common Files\\Microsoft Shared\\Ink\\tabtip.exe");


	//void CreateLowProcess();
	//void SetLowLabelToFile();
	//void ShowProcessIntegrityLevel();


	//AfxMessageBox(_T("FinalConstruct() :: CreateProcess"));
	LaunchViaCreateProcess(program3.c_str(), _T(""));				//ok

	//AfxMessageBox(_T("FinalConstruct() :: ShellExecuteEx"));
	LaunchViaShellExecute(program2.c_str(), _T(""));				// nothing 

	//AfxMessageBox(_T("FinalConstruct() :: Util::Module::ExecuteBlock - runas"));
	LaunchViaExecuteBlock(program2.c_str(), _T(""));				// permisssion error	

	//AfxMessageBox(_T("FinalConstruct() :: ShellExecute - open"));
	::ShellExecute(NULL, "open", program3.c_str(), "", NULL, SW_SHOWNORMAL);	// ok
	*/

	::MessageBox(NULL, "end of test", "test", MB_OK);

	return 0;
}


bool TestCFile(tstring filename)
{
	PrintMessage(_T("CFile test >>>>>>>>>"));
	PrintMessage(filename.c_str());

	// read & write 
	TCHAR	szBuffer[256]; 
	UINT    nActual = 0; 
	CFile	myFile;

	if (myFile.Open(filename.c_str(), CFile::modeCreate | CFile::modeReadWrite, NULL ) )
	{
		PrintMessage(_T("success : CFile::modeCreate | CFile::modeReadWrite"));

		myFile.Write( szBuffer, sizeof( szBuffer ) ); 
		myFile.Flush();
		myFile.Seek( 0, CFile::begin );
		nActual = myFile.Read( szBuffer, sizeof( szBuffer ) ); 
		myFile.Close();
	}


	// status
	CFile theFile;
	BOOL bOpenOK;

	CFileStatus status;
	if( CFile::GetStatus( filename.c_str(), status ) )
	{
		if (bOpenOK = theFile.Open( filename.c_str(), CFile::modeRead ))
			PrintMessage(_T("success : Status/Open the file with the modeRead flag"));
	}
	else
	{
		if (bOpenOK = theFile.Open( filename.c_str(), CFile::modeRead))
			PrintMessage(_T("success : Open the file with the modeRead flag"));
	}

	if (bOpenOK)
		theFile.Close();
	else
		PrintMessage(_T("fail : Open the file"));

	return true;
}


bool TestIOStream(tstring filename)
{
	PrintMessage(_T("iostream test >>>>>>>>>"));
	PrintMessage(filename.c_str());

	// input stream
	ifstream t(filename.c_str());
	tstring str((std::istreambuf_iterator<TCHAR>(t)), std::istreambuf_iterator<TCHAR>());
	PrintMessage(str.c_str());

	// output stream
	ofstream ofs;
	ofs.open (filename.c_str(), std::ofstream::out | std::ofstream::app);
	ofs << " more lorem ipsum";
	ofs.close();

	return true;
}

bool TestCreateFile(tstring filename)
{
	PrintMessage(_T("CreateFile test >>>>>>>>>"));
	PrintMessage(filename.c_str());

	// CreateFile
	// Creates the new file to write to for the upper-case version.
	HANDLE hTempFile = CreateFile((LPCTSTR)(filename.c_str()), // file name 
						   GENERIC_WRITE,        // open for write 
						   0,                    // do not share 
						   NULL,                 // default security 
						   CREATE_ALWAYS,        // overwrite existing
						   FILE_ATTRIBUTE_NORMAL,// normal file 
						   NULL);                // no template 
	if (hTempFile == INVALID_HANDLE_VALUE) 
	{ 
		PrintMessage(_T("fail : First CreateFile failed"));
		//return false;
	}
	else
	{
		PrintMessage(_T("success : Creates the new file to write"));
	}
	CloseHandle(hTempFile);


	// Opens the existing file. 
	HANDLE hFile = CreateFile((LPCTSTR)(filename.c_str()),// file name 
					   GENERIC_READ,          // open for reading 
					   0,                     // do not share 
					   NULL,                  // default security 
					   OPEN_EXISTING,         // existing file only 
					   FILE_ATTRIBUTE_NORMAL, // normal file 
					   NULL);                 // no template 
	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		PrintMessage(_T("fail : Second CreateFile failed"));
		return false;
	}
	PrintMessage(_T("success : Opens the existing file"));
	CloseHandle(hFile);

	return true;
}

bool TestFOpen(tstring filename)
{
	PrintMessage(_T("fopen test >>>>>>>>>"));
	PrintMessage(filename.c_str());

	// fopen with r flag
	FILE* pFile = NULL;
	char mystring[100] = {0, };

	pFile = fopen(filename.c_str(), "r");
	if (pFile == NULL) 
	{
		PrintMessage(_T("fail : fopen with r flag"));
		return false;
	}
	else 
	{
		PrintMessage(_T("success : fopen with r flag"));

		if (fgets(mystring, 100, pFile) != NULL)
			PrintMessage((LPCTSTR)mystring);
		fclose(pFile);
	}

	// fopen with w+ flag
	pFile = NULL;
	pFile = fopen(filename.c_str(), "w+");
	if (pFile == NULL)
	{
		PrintMessage(_T("fail : fopen with w+ flag"));
		return false;
	}
	else  
	{
		PrintMessage(_T("success : fopen with w+ flag"));
		fputs("fopen example", pFile);
		fclose(pFile);
	}


	return true;
}

bool DeleteFile(tstring filename)
{
	PrintMessage(_T("DeleteFile >>>>>>>>>"));
	PrintMessage(filename.c_str());

	if (::DeleteFile(filename.c_str()))
	{
		PrintMessage(_T("success : deleted the file"));
		PrintMessage(_T(" "));
		return true;
	}
	else
	{
		PrintMessage(_T("fail : cannot delete the file"));
		PrintMessage(_T(" "));
		return false;
	}
}

void PrintMessage(LPCTSTR msg)
{
	cout << msg << endl;
	
	// AfxMessageBox(msg);
	
	// ::OutputDebugString(msg);
}

HANDLE LaunchViaCreateProcess(LPCTSTR program, LPCTSTR args)
{
	HANDLE hProcess = NULL;

	STARTUPINFO	si;
	PROCESS_INFORMATION pi;

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	int ret = ::CreateProcess(NULL, (LPTSTR) program, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (ret == 0) {
		::MessageBox(NULL, "CreateProcess 실패", "install test", MB_OK);
	}
	else {
		hProcess = pi.hProcess;
	}

	/*
	HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	if(::CreateProcess(program, (LPTSTR)args,
	NULL,  // process security
	NULL,  // thread security
	FALSE, // no inheritance
	0,     // no startup flags
	NULL,  // no special environment
	NULL,  // default startup directory
	&startupInfo,
	&processInfo))
	{
	hProcess = processInfo.hProcess;
	}
	*/

	return hProcess;
}

HANDLE LaunchViaShellExecute(LPCTSTR program, LPCTSTR arg)
{
	HANDLE hProcess = NULL;
    SHELLEXECUTEINFO shellInfo;
    ::ZeroMemory(&shellInfo, sizeof(shellInfo));
    shellInfo.cbSize = sizeof(shellInfo);
    shellInfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
    shellInfo.lpFile = program;
    shellInfo.lpParameters = arg;
    if(::ShellExecuteEx(&shellInfo))
    { /* success */
        hProcess = shellInfo.hProcess;
    } /* success */

    return hProcess;
}

bool LaunchViaExecuteBlock(LPCTSTR program, LPCTSTR arg)
{
	/*
	if (CComUtil::IsExecutableFile(program) == true) {
		bool res = Util::Module::ExecuteBlock(program, arg);
		if (res == false)
			::MessageBox(NULL, "RES_UNKNOWN", "install test", MB_OK);
		else
			::MessageBox(NULL, "RES_SUCCESS", "install test", MB_OK);
	}
	else if (CComUtil::IsComBinaryFile(program) == true)
	{
		//OnlyOneFileCabInstall(cabpath);
		::MessageBox(NULL, "OnlyOneFileCabInstall", "install test", MB_OK);
	}
	else {//if(CCabDecompress::IsCab(cabpath) == FALSE) {
		::MessageBox(NULL, "올바른 CAB파일이 아님", "install test", MB_OK);
	}
	*/

	return true;
}

HRESULT SaveFileData(
         LPWSTR pszName,    // Desired file name and path
         BYTE* pbData,      // Data for save
         UINT cbBytes,      // Count of bytes in pbData
         LPWSTR* ppszDest)  // OUT: final save location
{
   LPWSTR pszTempPath;
   HRESULT hr;
   
   hr = IEGetWriteableFolderPath(
         FOLDERID_InternetCache,   // Temporary Internet Files
         &pszTempPath);
      
   if (S_OK == hr) 	// S_FALSE means no access to folder
   {
      // Create a temporary file. 
      wchar_t szTempName[MAX_PATH];
      GetTempFileNameW(
             pszTempPath, // Directory for temp files 
             L"tmp",   // Temp file name prefix 
             0,           // Create unique name 
             szTempName); // Buffer for name 
      
      HANDLE hTempFile;
      hTempFile = CreateFileW(
             (LPWSTR)szTempName,           // File name 
             GENERIC_READ | GENERIC_WRITE, // Open for read/write 
             0,                            // Do not share 
             NULL,                         // Default security 
             CREATE_ALWAYS,                // Overwrite existing file
             FILE_ATTRIBUTE_NORMAL,        // Normal file 
             NULL);                        // No template 
      
      if (INVALID_HANDLE_VALUE != hTempFile) 
      { 
         DWORD dwBytesWritten;
         
         // Save the file
         WriteFile(hTempFile, pbData, cbBytes, 
                   &dwBytesWritten, NULL); 
         ASSERT(dwBytesWritten == cbBytes);
         CloseHandle(hTempFile);
         
         // Show the SaveAs dialog
         hr = SaveFileProtected(pszName, (LPWSTR)szTempName, ppszDest);
         
         DeleteFileW((LPCWSTR)szTempName);
      }
      else
      {
         // Could not create temporary file
         hr = HRESULT_FROM_WIN32(GetLastError());
      }
	  
      // Free memory
      CoTaskMemFree(pszTempPath);
   }
   
   return hr;
}
 
// RETURNS: S_OK, or S_FALSE if canceled.
// NOTE: Caller must free *ppszDest with CoTaskMemFree
HRESULT SaveFileProtected(
         LPWSTR pszName,     // Desired name and path
         LPWSTR pszSource,   // Current file location
         LPWSTR *ppszDest)   // OUT: path to saved file
{
   LPCWSTR pszExt = L"Text Files|*.txt|All Files|*.*|";
   LPCWSTR pszDefExt = L"txt";
   HANDLE  hState; // No need to call CloseHandle on this
   
   if (ppszDest) *ppszDest = NULL;
   
   HRESULT hr = IEShowSaveFileDialog(
          NULL,       // Application window (global)
          pszName,      // Initial filename and path
          NULL,         // Use path from filename
          pszExt,       // Extension filter list
          pszDefExt,    // Default extension
          1,            // Filter index
          OFN_ENABLESIZING | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT,
          ppszDest,     // Selected destination
          &hState);     // Used to complete the operation
         
   if (S_OK == hr)  // S_FALSE if user cancels the operation
   {
      hr = IESaveFile(hState, pszSource);
      hState = NULL;   // hState SHOULD NOT be used again; set to NULL
   }
   
   return hr;
}

void CreateLowProcess()
{
	BOOL bRet;
	HANDLE hToken;
	HANDLE hNewToken;

	// Notepad is used as an example
	WCHAR wszProcessName[MAX_PATH] =
		L"C:\\Windows\\System32\\Notepad.exe";

	// Low integrity SID
	WCHAR wszIntegritySid[20] = L"S-1-16-4096";
	PSID pIntegritySid = NULL;

	TOKEN_MANDATORY_LABEL TIL = { 0 };
	PROCESS_INFORMATION ProcInfo = { 0 };
	STARTUPINFO StartupInfo = { 0 };
	ULONG ExitCode = 0;

	if (OpenProcessToken(GetCurrentProcess(), MAXIMUM_ALLOWED, &hToken))
	{
		if (DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityImpersonation, TokenPrimary, &hNewToken))
		{
			if (ConvertStringSidToSidW(wszIntegritySid, &pIntegritySid))
			{
				TIL.Label.Attributes = SE_GROUP_INTEGRITY;
				TIL.Label.Sid = pIntegritySid;

				// Set the process integrity level
				if (SetTokenInformation(hNewToken, TokenIntegrityLevel, &TIL,
					sizeof(TOKEN_MANDATORY_LABEL) +GetLengthSid(pIntegritySid)))
				{
					// Create the new process at Low integrity
					bRet = CreateProcessAsUserW(hNewToken, NULL, wszProcessName, NULL, NULL, FALSE,
						0, NULL, NULL, (LPSTARTUPINFOW)&StartupInfo, &ProcInfo);
				}

				LocalFree(pIntegritySid);
			}
			CloseHandle(hNewToken);
		}
		CloseHandle(hToken);
	}
}

void SetLowLabelToFile()
{
	// The LABEL_SECURITY_INFORMATION SDDL SACL to be set for low integrity 
#define LOW_INTEGRITY_SDDL_SACL_W L"S:(ML;;NW;;;LW)"
	DWORD dwErr = ERROR_SUCCESS;
	PSECURITY_DESCRIPTOR pSD = NULL;

	PACL pSacl = NULL; // not allocated
	BOOL fSaclPresent = FALSE;
	BOOL fSaclDefaulted = FALSE;
	LPCWSTR pwszFileName = L"Sample.txt";

	if (ConvertStringSecurityDescriptorToSecurityDescriptorW(
		LOW_INTEGRITY_SDDL_SACL_W, SDDL_REVISION_1, &pSD, NULL))
	{
		if (GetSecurityDescriptorSacl(pSD, &fSaclPresent, &pSacl,
			&fSaclDefaulted))
		{
			// Note that psidOwner, psidGroup, and pDacl are 
			// all NULL and set the new LABEL_SECURITY_INFORMATION
			dwErr = SetNamedSecurityInfoW((LPWSTR) pwszFileName,
				SE_FILE_OBJECT, LABEL_SECURITY_INFORMATION,
				NULL, NULL, NULL, pSacl);
		}
		LocalFree(pSD);
	}
}

void ShowProcessIntegrityLevel()
{
	HANDLE hToken;
	HANDLE hProcess;

	DWORD dwLengthNeeded;
	DWORD dwError = ERROR_SUCCESS;

	PTOKEN_MANDATORY_LABEL pTIL = NULL;
	LPWSTR pStringSid;
	DWORD dwIntegrityLevel;

	hProcess = GetCurrentProcess();
	if (OpenProcessToken(hProcess, TOKEN_QUERY |
		TOKEN_QUERY_SOURCE, &hToken))
	{
		// Get the Integrity level.
		if (!GetTokenInformation(hToken, TokenIntegrityLevel,
			NULL, 0, &dwLengthNeeded))
		{
			dwError = GetLastError();
			if (dwError == ERROR_INSUFFICIENT_BUFFER)
			{
				pTIL = (PTOKEN_MANDATORY_LABEL) LocalAlloc(0,
					dwLengthNeeded);
				if (pTIL != NULL)
				{
					if (GetTokenInformation(hToken, TokenIntegrityLevel,
						pTIL, dwLengthNeeded, &dwLengthNeeded))
					{
						dwIntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid,
							(DWORD) (UCHAR) (*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1));

						if (dwIntegrityLevel < SECURITY_MANDATORY_MEDIUM_RID)
						{
							// Low Integrity
							wprintf(L"Low Process");
						}
						else if (dwIntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID &&
							dwIntegrityLevel < SECURITY_MANDATORY_HIGH_RID)
						{
							// Medium Integrity
							wprintf(L"Medium Process");
						}
						else if (dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID)
						{
							// High Integrity
							wprintf(L"High Integrity Process");
						}
					}
					LocalFree(pTIL);
				}
			}
		}
		CloseHandle(hToken);
	}
}

