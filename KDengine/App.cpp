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
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 40.0f ) );
}

void App::Update()
{
	pl.Bind( wnd.Gfx(), cam.GetMatrix() );
	CheckMouseInWin();

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cam.Translate({ 0.0f,0.0f,deltaTime });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ -deltaTime,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-deltaTime });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ deltaTime,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			cam.Translate({ 0.0f,deltaTime,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			cam.Translate({ 0.0f,-deltaTime,0.0f });
		}
	}
	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}
}

void App::DoFrame()
{
	gobber.Draw( wnd.Gfx() );
	pl.Draw( wnd.Gfx() );

	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
	gobber.ShowWindow( wnd.Gfx(), "Gobber" );
	
}
