#include "Window.h"
#include "App.h"

int CALLBACK WinMain(
	HINSTANCE hInst,
	HINSTANCE hPrev,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	try
	{
		return App{}.Go();
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