#include "App.h"
#include "GDIPlusManager.h"
#include "KDMath.h"
#include <random>

GDIPlusManager gdipm; 
namespace dx = DirectX;

App::App()
	:
	wnd( 1280, 720, "KDEngine App" ),
	pl( wnd.Gfx(), 1.0f )
{}

void App::Update()
{
	pl.Bind( wnd.Gfx(), cam.GetMatrix() );
}

void App::ComposeFrame()
{
	nano.Draw( wnd.Gfx() );
	pl.Draw( wnd.Gfx() );

	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
	nano.ShowWindow( "Nano" );
	ShowRawInputWindow();
}
