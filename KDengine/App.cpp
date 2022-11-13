#include "App.h"
#include "GDIPlusManager.h"
#include "KDMath.h"
#include <random>

GDIPlusManager gdipm; 
namespace dx = DirectX;

App::App()
	:
	wnd( 1280, 720, "KDEngine App" ),
	pl( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 40.0f ) );
}

void App::Update()
{
	dt = timer.Mark() * speedF;
	wnd.Gfx().SetCamera( cam.GetMatrix() );
	pl.Bind( wnd.Gfx(), cam.GetMatrix() );
}

void App::ComposeFrame()
{
	nano.Draw( wnd.Gfx() );
	pl.Draw( wnd.Gfx() );

	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
	nano.ShowWindow( "Nano" );
}
