#include <afx.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <windows.h>
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


int main() 
{
	// test path
	vector<tstring> testpath;
	testpath.push_back(_T("M:\\"));

	tstring filename = _T("test.txt");

	// file access test
	for (int i = 0; i < testpath.size(); i++) 
	{
		//TestCFile(testpath.at(i) + filename);
		//TestIOStream(testpath.at(i) + filename);
		TestCreateFile(testpath.at(i) + filename);
		TestFOpen(testpath.at(i) + filename);
	}

	// delete
	for (int i = 0; i < testpath.size(); i++)
	{
		//DeleteFile(testpath.at(i) + filename);
	}

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

