#include "App.h"
#include "Cone.h"
#include "Cube.h"
#include "Plane.h"
#include "Prism.h"
#include "Sphere.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "Sheet.h"
#include "BoxChkTex.h"
#include <random>

GDIPlusManager gdipm;

App::App()
	:
	wnd(800, 600, "KDEngine App")
{
	box = std::make_unique<BoxChkTex>( wnd.Gfx(), Cube::MakeTextured<BoxChkTex::Vertex>(), wnd.kbd, "Images\\cube.png" );

	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 40.0f ) );
}

void App::Update()
{
	dt = timer.Mark();

	box->Update( dt );
}

void App::ComposeFrame()
{
	box->Draw( wnd.Gfx() );
}
