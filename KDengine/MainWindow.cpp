#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInst,
	HINSTANCE hPrev,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	Window wnd( 800, 600, "DirectX Test" );

	MSG msg;
	BOOL gResult;

	while ( (gResult = GetMessage(&msg, nullptr, 0, 0)) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}


	if ( gResult == -1 )
		return -1;
	return msg.wParam;
}