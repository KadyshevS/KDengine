#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Drawable.h"
#include "ImguiManager.h"
#include "PointLight.h"
#include "Camera.h"
#include "Mesh.h"
#include "TestCube.h"
#include "ScriptCommander.h"
#include "FrameCommander.h"
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
			fComm.Execute( wnd.Gfx() );
			wnd.Gfx().EndFrame();
			fComm.Reset();
		}
	}
	void ShowSimulationWindow() noexcept
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
	void UpdateMouse()
	{
		CheckMouseInWin();

		if (!wnd.CursorEnabled())
		{
			speedCam = wnd.kbd.KeyIsPressed(VK_SHIFT) ? 1.0f : 2.0f;

			if (wnd.kbd.KeyIsPressed('W'))
			{
				cam.Translate({ 0.0f,0.0f,speedCam * deltaTime });
			}
			if (wnd.kbd.KeyIsPressed('A'))
			{
				cam.Translate({ speedCam * -deltaTime,0.0f,0.0f });
			}
			if (wnd.kbd.KeyIsPressed('S'))
			{
				cam.Translate({ 0.0f,0.0f,speedCam * -deltaTime });
			}
			if (wnd.kbd.KeyIsPressed('D'))
			{
				cam.Translate({ speedCam * deltaTime,0.0f,0.0f });
			}
			if (wnd.kbd.KeyIsPressed('R'))
			{
				cam.Translate({ 0.0f,speedCam * deltaTime,0.0f });
			}
			if (wnd.kbd.KeyIsPressed('F'))
			{
				cam.Translate({ 0.0f,speedCam * -deltaTime,0.0f });
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
	ScriptCommander sComm;
	FrameCommander	fComm;
	Window			wnd;
	FrameTimer		timer;
	Camera			cam;
	PointLight		pl;

//	Model		sponza{ wnd.Gfx(), "Models\\sponza\\sponza.obj", 1.0f / 20.f };
	TestCube	cube1{ wnd.Gfx(), 1.0f };
	TestCube	cube2{ wnd.Gfx(), 1.0f };
};