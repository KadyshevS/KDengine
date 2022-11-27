#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Drawable.h"
#include "ImguiManager.h"
#include "PointLight.h"
#include "Camera.h"
#include "Mesh.h"
#include "TestPlane.h"
#include "TestCube.h"
#include <set>

class App
{
public:
	App( const std::string& commandLine = "" );
	~App() = default;
	int Go()
	{
		while (true)
		{
		//	Process Messages every frame
			if (const auto ecode = Window::ProcessMessages())
			{
				// if return optional has value, means we're quitting so return exit code
				return *ecode;
			}

		//	Updating deltaTime
			deltaTime = timer.Mark() * speedF;
		//	Updating camera position
			wnd.Gfx().SetCamera( cam.GetMatrix() );
		//	Updating game logic
			Update();
		//	Draw
			wnd.Gfx().BeginFrame( 0.0f, 0.0f, 0.3f );
			DoFrame();
			wnd.Gfx().EndFrame();
		}
	}
	void SpawnSimulationWindow() noexcept
	{
		if (ImGui::Begin("Simulation Speed"))
		{
			ImGui::SliderFloat("Speed Factor", &speedF, 0.0f, 6.0f, "%.4f", 3.2f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Status: %s %s", deltaTime == 0.0f ? "STOPPED" : "RUNNING", "(hold Space to stop)");
		}
		ImGui::End();
	}
	void ShowDemoWindow() noexcept
	{
		if ( showDemoWin )
		{
			ImGui::ShowDemoWindow( &showDemoWin );
		}
	}
	void CheckMouseInWin() noexcept
	{
		while (const auto e = wnd.kbd.ReadKey())
		{
			if (!e->IsPress())
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
	}

private:
	void Update();
	void DoFrame();

private:
	bool	showDemoWin = false;
	float	deltaTime	= 0.0f;
	float	speedF		= 1.0f;
	float	speedCam	= 4.0f;

private:
	std::string		commandLine;
	ImguiManager	imgui;
	Window			wnd;
	FrameTimer		timer;
	Camera			cam;
	PointLight		pl;

	Model	sponza{ wnd.Gfx(), "Models\\sponza\\sponza.obj" };
};