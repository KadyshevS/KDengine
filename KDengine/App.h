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
		wnd.EnableCursor();
		wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
		while (true)
		{
			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
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
			ImGui::Text("Status: %s %s", dt == 0.0f ? "STOPPED" : "RUNNING", "(hold Space to stop)");
		}
		ImGui::End();
	}

private:
	void Update();
	void ComposeFrame();

private:
	float dt		= 0.0f;
	float speedF	= 1.0f;

private:
	ImguiManager	imgui;
	Window			wnd;
	FrameTimer		timer;
	Camera			cam;
	PointLight		pl;

	Model			nano{ wnd.Gfx(), "Models\\nano_hierarchy.gltf" };
};