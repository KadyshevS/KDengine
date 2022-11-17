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

	//	Input capture
	while ( const auto e = wnd.kbd.ReadKey() )
	{
		if ( !e->IsPress() )
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWin = true;
			break;
		}
	}

	if ( !wnd.CursorEnabled() )
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

void App::ComposeFrame()
{
	nano.Draw( wnd.Gfx() );
	nano2.Draw( wnd.Gfx() );
	pl.Draw( wnd.Gfx() );

	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
	nano.ShowWindow("Nano 1");
	nano2.ShowWindow("Nano 2");
}
