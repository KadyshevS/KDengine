#include "App.h"
#include "GDIPlusManager.h"
#include "KDMath.h"
#include <random>
#include "NormalMapTweaker.h"
#include <shellapi.h>

GDIPlusManager gdipm; 
namespace dx = DirectX;

App::App( const std::string& commandLine )
	:
	commandLine( commandLine ),
	wnd( 1280, 720, "KDEngine App" ),
	pl( wnd.Gfx(), 1.0f )
{
	if ( this->commandLine != "" )
	{
		int nArgs;
		const auto pLineW = GetCommandLineW();
		const auto pArgs = CommandLineToArgvW(pLineW, &nArgs);
		if (nArgs >= 4 && std::wstring(pArgs[1]) == L"--ntwerk-rotx180")
		{
			const std::wstring pathInWide = pArgs[2];
			const std::wstring pathOutWide = pArgs[3];
			NormalMapTweaker::RotateXAxis180(
				std::string(pathInWide.begin(), pathInWide.end()),
				std::string(pathOutWide.begin(), pathOutWide.end())
			);
			throw std::runtime_error("Normal map processed successfully. Just kidding about that whole runtime error thing.");
		}
	}

	sponza.SetRootTransform( dx::XMMatrixScaling(0.01f, 0.01f, 0.01f) );
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 400.0f ) );
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
	pl.Draw( wnd.Gfx() );
	sponza.Draw(wnd.Gfx());

	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
}
