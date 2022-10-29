#include "Window.h"
#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInst,
	HINSTANCE hPrev,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	try
	{
		Window wnd(800, 600, "DirectX Test");

		MSG msg;
		BOOL gResult;

		while ( gResult = GetMessage(&msg, nullptr, 0, 0) > 0 )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			while ( !wnd.mouse.IsEmpty() )
			{
				const auto e = wnd.mouse.Read();
				if ( e.GetType() == Mouse::Event::Type::Move )
				{
					std::ostringstream oss;
					oss << "(" << wnd.mouse.GetPosX() << "; " << wnd.mouse.GetPosY() << ")";
					wnd.SetTitle( oss.str() );
				}
			}
		}

		return msg.wParam;
	}
	catch ( const KDException& e )
	{
		MessageBox( nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( const std::exception& e )
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( ... )
	{
		MessageBox(nullptr, "No details available", "Unknown Error", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}