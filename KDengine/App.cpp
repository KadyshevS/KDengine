#include "App.h"
#include "GDIPlusManager.h"
#include "KDMath.h"
#include "Box.h"
#include "Cube.h"
#include "Prism.h"
#include "Cylinder.h"
#include <random>

GDIPlusManager gdipm; 

App::App()
	:
	wnd( 800, 600, "KDEngine App" ),
	pl( wnd.Gfx() )
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, PI * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, PI * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, PI * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	std::uniform_real_distribution<float> cdist(0.0f, 1.0f);
	std::uniform_int_distribution<int> tdist(5, 15);

	boxes.reserve( boxCount );
	cils.reserve( cilCount );

	for ( int i = 0; i < boxCount; i++ )
	{
		const DirectX::XMFLOAT3 mat = { cdist(rng), cdist(rng), cdist(rng) };
		boxes.push_back(
			std::make_unique<Box>(
				wnd.Gfx(), rng, adist, ddist, odist, rdist, mat
			)
		);
	}
	for (int i = 0; i < cilCount; i++)
	{
		cils.push_back(
			std::make_unique<Cylinder>(
				wnd.Gfx(), rng, adist, ddist, odist, rdist, tdist
			)
		);
	}

	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 40.0f ) );
}

void App::Update()
{
	dt = timer.Mark() * speedF;
	wnd.Gfx().SetCamera( cam.GetMatrix() );
	pl.Bind( wnd.Gfx(), cam.GetMatrix() );

	if ( wnd.kbd.KeyIsPressed(VK_SPACE) )
	{
		dt = 0.0f;
	}

	for ( auto& b : boxes )
	{
		b->Update( dt );
	}
	for ( auto& c : cils )
	{
		c->Update( dt );
	}
}

void App::ComposeFrame()
{
	for ( auto& b : boxes )
	{
		b->Draw( wnd.Gfx() );
	}
	for ( auto& c : cils )
	{
		c->Draw( wnd.Gfx() );
	}
	pl.Draw( wnd.Gfx() );

	if ( ImGui::Begin( "Simulation Speed" ) )
	{
		ImGui::SliderFloat( "Speed Factor", &speedF, 0.0f, 6.0f, "%.4f", 3.2f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "Status: %s %s", dt == 0.0 ? "STOPPED" : "RUNNING", "(hold Space to stop)");
	}
	ImGui::End();
	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
}
