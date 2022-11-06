#include "App.h"
#include "Cube.h"
#include "GDIPlusManager.h"
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
	dt = timer.Mark() * speedF;

	if ( wnd.kbd.KeyIsPressed(VK_SPACE) )
	{
		dt = 0.0f;
	}

	box->Update( dt );
}

void App::ComposeFrame()
{
	box->Draw( wnd.Gfx() );

	static char buffer[1024];
	if ( ImGui::Begin( "Simulation Speed" ) )
	{
		ImGui::SliderFloat( "Speed Factor", &speedF, 0.5f, 4.0f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::InputText( "Sample Text", buffer, sizeof(buffer) );
	}
	ImGui::End();
}
