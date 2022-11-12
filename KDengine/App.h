#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Drawable.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class App
{
	ImguiManager imgui;
	Window wnd;
	FrameTimer timer;
	Camera cam;
	PointLight pl;

	float dt;
	float speedF = 1.0f;
	bool show_demo_window = false;

	std::vector<std::unique_ptr<Drawable>> boxes;

	static constexpr unsigned int boxCount = 45;
	static constexpr unsigned int cilCount = 45;
	static constexpr unsigned int pyrmsCount = 45;
	static constexpr unsigned int tboxCount = 45;

public:
	App();
	int Go()
	{
		while (true)
		{
			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
			}
			Update();

			wnd.Gfx().BeginFrame( 0.0f, 0.0f, 0.3f );
			ComposeFrame();
			// Imgui
			static bool show_demo_window = false;
			if ( show_demo_window )
			{
				ImGui::ShowDemoWindow(&show_demo_window);
			}
			wnd.Gfx().EndFrame();
		}
	}
	void SpawnFrameSpeedWin() noexcept
	{
		if (ImGui::Begin("Simulation Speed"))
		{
			ImGui::SliderFloat("Speed Factor", &speedF, 0.0f, 6.0f, "%.4f", 3.2f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Status: %s %s", dt == 0.0f ? "STOPPED" : "RUNNING", "(hold Space to stop)");
		}
		ImGui::End();
	}
	~App() = default;

private:
	void Update();
	void ComposeFrame();
};