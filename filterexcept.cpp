////////////////////////////////////////////////////////////////////////////////
//
// FilterExcept.cpp 
//
// Author: Oleg Starodumov (www.debuginfo.com)
//


////////////////////////////////////////////////////////////////////////////////
//
// This example demonstrates how to call a custom filter inside __except clause, 
// and thus make it possible to debug the filter 
//


////////////////////////////////////////////////////////////////////////////////
// Include files 
//

#include <windows.h>
#include <tchar.h>
#include <dbghelp.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// Directives
//

// Link with DbgHelp.lib 
#pragma comment ( lib, "dbghelp.lib" )


////////////////////////////////////////////////////////////////////////////////
// Function declarations
//

LONG __stdcall MyCustomFilter( EXCEPTION_POINTERS* pep ); 
void CreateMiniDump( EXCEPTION_POINTERS* pep ); 
void FaultyFunc(); 


////////////////////////////////////////////////////////////////////////////////
// main() function
//

int main() 
{
	//SetUnhandledExceptionFilter( MyCustomFilter ); 

	// Here we call the filter in __except clause 

	__try
	{
		FaultyFunc();
	}
	__except( MyCustomFilter( GetExceptionInformation() ) )
	{
		_tprintf( _T("Exception handled.\n") );
	}

	return 0; 
}


////////////////////////////////////////////////////////////////////////////////
// Custom filter for unhandled exceptions
//

LONG __stdcall MyCustomFilter( EXCEPTION_POINTERS* pep ) 
{
	CreateMiniDump( pep ); 

	return EXCEPTION_EXECUTE_HANDLER; 
}


////////////////////////////////////////////////////////////////////////////////
// CreateMiniDump() function
//

void CreateMiniDump( EXCEPTION_POINTERS* pep ) 
{
	// Open the file 

	HANDLE hFile = CreateFile( _T("MiniDump.dmp"), GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE; 

		MINIDUMP_TYPE mdt       = MiniDumpNormal; 

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 ); 

		if( !rv ) 
			_tprintf( _T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError() ); 
		else 
			_tprintf( _T("Minidump created.\n") ); 

		// Close the file 

		CloseHandle( hFile ); 

	}
	else 
	{
		_tprintf( _T("CreateFile failed. Error: %u \n"), GetLastError() ); 
	}

}


////////////////////////////////////////////////////////////////////////////////
// Faulty function
//

void FaultyFunc()
{
	_tprintf( _T("We will crash now...\n") );

	*(int*)0 = 1;
}

