#include "App.h"
#include <random>

App::App()
	:
	wnd(800, 600, "KDEngine App")
{
	std::mt19937 rng( std::random_device{}() );
	std::uniform_real_distribution<float> adist( 0.0f, 3.1415f * 2.0f );
	std::uniform_real_distribution<float> ddist( 0.0f, 3.1415f * 2.0f );
	std::uniform_real_distribution<float> odist( 0.0f, 3.1415f * 0.3f );
	std::uniform_real_distribution<float> rdist( 6.0f, 20.0f );

	for ( size_t i = 0; i < boxCount; i++ )
	{
		boxes.push_back( std::make_unique<Box>(wnd.Gfx(), rng, adist, ddist, odist, rdist) );
	}

	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 40.0f ) );
}

void App::Update()
{
	dt = timer.Mark();

	for ( auto& b : boxes )
	{
		b->Update( dt );
	}
}

void App::ComposeFrame()
{
	for ( auto& b : boxes )
	{
		b->Draw( wnd.Gfx() );
	}
}
