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
	sComm( TokenizeQuoted(commandLine) ),
	wnd( 1280, 720, "KDEngine App" ),
	pl( wnd.Gfx(), 1.0f )
{
	cube1.SetPos( { 4.0f, 0.0f, 0.0f } );
	cube1.SetPos( { 0.0f, 4.0f, 0.0f } );
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 400.0f ) );
}

void App::Update()
{
	pl.Bind( wnd.Gfx(), cam.GetMatrix() );
	pl.Submit( fComm );
	cube1.Submit( fComm );
	cube2.Submit( fComm );

	UpdateMouse();
}

void App::DoFrame()
{
	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
//	sponza.ShowWindow( wnd.Gfx(), "Crytek Sponza scene" );
	cube1.SpawnControlWindow( wnd.Gfx(), "Cube 1" );
	cube2.SpawnControlWindow( wnd.Gfx(), "Cube 2" );
}
