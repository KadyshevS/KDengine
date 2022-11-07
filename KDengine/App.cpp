#include "App.h"
#include "GDIPlusManager.h"
#include "KDMath.h"
#include "Box.h"
#include "Cube.h"
#include <random>

GDIPlusManager gdipm; 

App::App()
	:
	wnd(800, 600, "KDEngine App"),
	pl( wnd.Gfx() )
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, PI * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, PI * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, PI * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	boxes.reserve( boxCount );

	for ( int i = 0; i < boxCount; i++ )
	{
		boxes.push_back( 
			std::make_unique<Box>(
				wnd.Gfx(), rng, adist, ddist, odist, rdist
			) 
		);
	}

	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 40.0f ) );
}

void App::Update()
{
	dt = timer.Mark() * speedF;
	wnd.Gfx().SetCamera( cam.GetMatrix() );

	if ( wnd.kbd.KeyIsPressed(VK_SPACE) )
	{
		dt = 0.0f;
	}

	for ( auto& b : boxes )
	{
		b->Update( dt );
	}
	pl.Bind( wnd.Gfx() );
}

void App::ComposeFrame()
{
	for ( auto& b : boxes )
	{
		b->Draw( wnd.Gfx() );
	}
	pl.Draw( wnd.Gfx() );

	if ( ImGui::Begin( "Simulation Speed" ) )
	{
		ImGui::SliderFloat( "Speed Factor", &speedF, 0.0f, 4.0f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "Status: %s %s", dt == 0.0 ? "STOPPED" : "RUNNING", "(hold Space to stop)");
	}
	ImGui::End();
	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
}
