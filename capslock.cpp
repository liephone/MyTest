#include <windows.h>

void setCapsLock(BOOL state);

int main() 
{ 
	setCapsLock(TRUE); 

	Sleep(2000);

	setCapsLock(FALSE);

	return 0;
}

void setCapsLock(BOOL state)
{
	BYTE keyState[256] = {0, };

	GetKeyboardState((LPBYTE)&keyState);

	if( (state && !(keyState[VK_NUMLOCK] & 1)) ||
          (!state && (keyState[VK_NUMLOCK] & 1)) )
	{
		// Simulate a key press
		keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | 0,
                      0 );
      
		// Simulate a key release
		keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                      0);
      }
}
