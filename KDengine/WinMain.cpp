#include <Windows.h>

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int CALLBACK WinMain(
	HINSTANCE hInst,
	HINSTANCE hPrev,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	const char* pClassName = "kd3d";
//	Register Window class
	WNDCLASSEXA wc = {0};
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;

	RegisterClassEx( &wc );

//	Create Window class
	HWND hWnd = CreateWindowEx(
		0, pClassName, "KD Engine",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInst, nullptr
	);

//	Show Window
	ShowWindow( hWnd, SW_SHOW );

//	Translate Messages
	MSG msg;
	BOOL gResult;
	while ( gResult = GetMessage(&msg, nullptr, 0, 0) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	if ( gResult == -1 )
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}