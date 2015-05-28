#include <windows.h>

typedef LRESULT (* message_callback)(HWND, WPARAM, LPARAM);
struct message_handler
{
	UINT message;
	message_callback handler;
};

static message_handler s_handlers[] = 
{
	{
		WM_PAINT, [] (HWND window, WPARAM, LPARAM) -> LRESULT
		{
			PAINTSTRUCT ps;
			VERIFY(BeginPaint(window, &ps));
			// Dress up some pixels here!
			EndPaint(window, &ps);
			return 0;
		}
	},
	{
		WM_DESTROY, [] (HWND, WPARAM, LPARAM) -> LRESULT
		{
			PostQuitMessage(0);
			return 0;
		}
	}
};

int __stdcall wWinMain(HINSTANCE module, HINSTANCE, PWSTR, int)
{
	// 1. WNDCLASS 정의
	WNDCLASS wc = {};
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hInstance = module;
	wc.lpszClassName = L"window";
	wc.lpfnWndProc = [] (HWND window, UINT message, 
						 WPARAM wparam, LPARAM lparam) -> LRESULT
	{
		for (auto h = s_handlers; h != s_handlers + _countof(s_handlers); ++h)
		{
			if (message == h->message)
			{
				return h->handler(window, wparam, lparam);
			}
		}
  
		return DefWindowProc(window, message, wparam, lparam);
	};


	// 2. RegisterClass 등록
	VERIFY(RegisterClass(&wc));


	// 3. CreateWindow 생성
	VERIFY(CreateWindow(wc.lpszClassName, L"Title",
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				nullptr, nullptr, module, nullptr));
	

	// message loop
	MSG message;
	BOOL result;

	while (result = GetMessage(&message, 0, 0, 0))
	{
		if (-1 != result)
		{
			DispatchMessage(&message);
		}
	}
}
