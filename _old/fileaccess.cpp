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


int main() 
{
	// test path
	vector<tstring> testpath;
	testpath.push_back(_T("C:\\"));
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
	
	tstring filename = _T("kks_test.dat");
	

	// file access test
	for (int i = 0; i < testpath.size(); i++) 
	{
		//TestCFile(testpath.at(i) + filename);
		//TestIOStream(testpath.at(i) + filename);
		TestCreateFile(testpath.at(i) + filename);
		//TestFOpen(testpath.at(i) + filename);
	}

	// delete
	for (int i = 0; i < testpath.size(); i++)
	{
		//DeleteFile(testpath.at(i) + filename);
	}


	//AfxMessageBox(_T("FinalConstruct() :: process test"));

	// process test 
	tstring program1 = _T("C:\\Users\\kskim\\AppData\\Local\\Packages\\windows_ie_ac_001\\AC\\delfino.exe");

	AfxMessageBox(_T("FinalConstruct() :: CreateProcess"));
	LaunchViaCreateProcess(program1.c_str(), _T(""));

	AfxMessageBox(_T("FinalConstruct() :: ShellExecuteEx"));
	LaunchViaShellExecute(program1.c_str(), _T(""));

	AfxMessageBox(_T("FinalConstruct() :: Util::Module::ExecuteBlock - runas"));
	LaunchViaExecuteBlock(program1.c_str(), _T(""));

	AfxMessageBox(_T("FinalConstruct() :: ShellExecute - open"));
	::ShellExecute(NULL, "open", program1.c_str(), "", NULL, SW_SHOWNORMAL);


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
	/*
	STARTUPINFO	si;
	PROCESS_INFORMATION pi;
	
	memset(&si,0,sizeof(si));
	memset(&pi,0,sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	int ret = ::CreateProcess(NULL,(LPSTR)exepath.c_str(),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	if(ret == 0) {
		::MessageBox(NULL, "CreateProcess 실패", "install test", MB_OK);
	}
	*/

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
    { /* success */
        hProcess = processInfo.hProcess;
    } /* success */

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
	if(CComUtil::IsExecutableFile(program) == true) {
		bool res = Util::Module::ExecuteBlock(program, arg);
		if(res == false)
			::MessageBox(NULL, "RES_UNKNOWN", "install test", MB_OK);
		else
			::MessageBox(NULL, "RES_SUCCESS", "install test", MB_OK);
	}
	else if(CComUtil::IsComBinaryFile(program) == true)
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

