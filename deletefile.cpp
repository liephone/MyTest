#include <windows.h>
#include <iostream>
#include <string>
using namespace std;


int main()
{
	string file = "c:\\Windows\\dokan_copy.dll";
	//string file = "test.txt";

	DWORD attr = ::GetFileAttributes(file.c_str());
	
	if ((attr & FILE_ATTRIBUTE_READONLY) != 0) {
		attr &= ~FILE_ATTRIBUTE_READONLY;
		BOOL bRes = ::SetFileAttributes(file.c_str(),attr);
		if (bRes)
			cout << "set file attr. successed.." << endl;
		else
			cout << "cannot set file attr. !!" << endl;
	}

	BOOL bResult = ::DeleteFile(file.c_str());
	if (bResult)
		cout << file.c_str() << " file deleted.." << endl;
	else
		cout << "cannot delete file !!" << endl;

	return 0;
}

