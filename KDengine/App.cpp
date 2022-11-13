#include "App.h"
#include "GDIPlusManager.h"
#include "KDMath.h"
#include "Box.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "BoxTex.h"
#include <random>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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

	boxes.reserve( boxCount+suzCount+pyrmsCount+tboxCount );

	for ( int i = 0; i < boxCount  ; i++ )
	{
		const DirectX::XMFLOAT3 mat = { cdist(rng), cdist(rng), cdist(rng) };
		boxes.push_back(
			std::make_unique<Box>(
				wnd.Gfx(), rng, adist, ddist, odist, rdist, mat
			)
		);
	}
	for ( int i = 0; i < suzCount  ; i++ )
	{
		DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
		suzs.push_back(
			std::make_unique<Suzanne>(
				wnd.Gfx(), rng, adist, ddist, odist, rdist, mat, 1.5f
			)
		);
	}
	for ( int i = 0; i < pyrmsCount; i++ )
	{
		pyrms.push_back(
			std::make_unique<Pyramid>(
				wnd.Gfx(), rng, adist, ddist, odist, rdist, tdist
			)
		);
	}
	for ( int i = 0; i < tboxCount ; i++ )
	{
		tboxes.push_back(
			std::make_unique<BoxTex>(
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
	pl.Bind( wnd.Gfx(), cam.GetMatrix() );

	if ( wnd.kbd.KeyIsPressed(VK_SPACE) )
	{
		dt = 0.0f;
	}

	for ( auto& b : boxes )
	{
		b->Update( dt );
	}
	for (auto& b : suzs)
	{
		b->Update(dt);
	}
	for (auto& b : pyrms)
	{
		b->Update(dt);
	}
	for (auto& b : tboxes)
	{
		b->Update(dt);
	}
}

void App::ComposeFrame()
{
	for ( auto& b : boxes )
	{
		b->Draw( wnd.Gfx() );
	}
	for ( auto& b : suzs )
	{
		b->Draw( wnd.Gfx() );
	}
	for ( auto& b : pyrms )
	{
		b->Draw( wnd.Gfx() );
	}
	for ( auto& b : tboxes )
	{
		b->Draw( wnd.Gfx() );
	}
	pl.Draw( wnd.Gfx() );

	SpawnSimulationWindow();
	cam.SpawnControlWindow();
	pl.SpawnControlWindow();
	SpawnBoxWindowManagerWindow();
	SpawnBoxWindows();
}
