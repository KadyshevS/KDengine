#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Drawable.h"
#include "ImguiManager.h"
#include "PointLight.h"
#include "Camera.h"
#include "Mesh.h"
#include <set>

class App
{
public:
	App();
	~App() = default;
	int Go()
	{
		wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
		while (true)
		{
			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
			}

			deltaTime = timer.Mark() * speedF;
			wnd.Gfx().SetCamera( cam.GetMatrix() );
			while (const auto e = wnd.kbd.ReadKey())
			{
				if (e->IsPress() && e->GetCode() == VK_INSERT)
				{
					if( wnd.CursorEnabled() )
					{
						wnd.DisableCursor();
						wnd.mouse.EnableRaw();
					}
					else
					{
						wnd.EnableCursor();
						wnd.mouse.DisableRaw();
					}
				}
			}
			Update();

			wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.3f);
			ComposeFrame();
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
	void ShowRawInputWindow() noexcept
	{
		while (const auto d = wnd.mouse.ReadRawDelta())
		{
			raw_x += d->x;
			raw_y += d->y;
		}
		if (ImGui::Begin("Raw Input"))
		{
			ImGui::Text( "Tally: (%d,%d)", raw_x, raw_y );
			ImGui::Text( "Cursor: %s",wnd.CursorEnabled() ? "enabled" : "disabled" );
		}
		ImGui::End();
	}

private:
	void Update();
	void ComposeFrame();

private:
	float	deltaTime	= 0.0f;
	float	speedF		= 1.0f;
	int		raw_x		= 0, 
			raw_y		= 0;

private:
	ImguiManager	imgui;
	Window			wnd;
	FrameTimer		timer;
	Camera			cam;
	PointLight		pl;

	Model			nano{ wnd.Gfx(), "Models\\nano_hierarchy.gltf" };
};