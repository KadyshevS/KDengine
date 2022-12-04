#include "App.h"
#include "KDMath.h"
#include <random>
#include "TexturePreprocessor.h"
#include "dxtex/DirectXTex.h"
#include <shellapi.h>
#include "QuoteUtil.h"

namespace dx = DirectX;

App::App( const std::string& commandLine )
	:
	commandLine( commandLine ),
	wnd( 1280, 720, "KDEngine App" ),
	sComm( TokenizeQuoted( commandLine ) ),
	pl( wnd.Gfx(), 0.5f )
{
	sponza.SetRootTransform( dx::XMMatrixScaling(0.01f, 0.01f, 0.01f) );
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 400.0f ) );
}

void App::Update()
{
	pl.Bind( wnd.Gfx(), cam.GetMatrix() );
	UpdateMouse();
}

void App::DoFrame()
{
	pl.Draw( wnd.Gfx() );
	sponza.Draw( wnd.Gfx() );

	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
}
