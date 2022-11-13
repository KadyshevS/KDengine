#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Drawable.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Box.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "BoxTex.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <set>

class App
{
public:
	App();
	~App() = default;
	int Go()
	{
		while (true)
		{
			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
			}
			Update();

			wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.3f);
			ComposeFrame();
			// Imgui
			static bool show_demo_window = false;
			if (show_demo_window)
			{
				ImGui::ShowDemoWindow(&show_demo_window);
			}
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
	void SpawnBoxWindowManagerWindow() noexcept
	{
		if (ImGui::Begin("Boxes"))
		{
			using namespace std::string_literals;
			const auto preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box..."s;
			if (ImGui::BeginCombo("Box Number", preview.c_str()))
			{
				for (int i = 0; i < boxes.size(); i++)
				{
					const bool selected = *comboBoxIndex == i;
					if (ImGui::Selectable(std::to_string(i).c_str(), selected))
					{
						comboBoxIndex = i;
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("Spawn Control Window") && comboBoxIndex)
			{
				boxControlIds.insert(*comboBoxIndex);
				comboBoxIndex.reset();
			}
		}
		ImGui::End();
	}
	void SpawnBoxWindows() noexcept
	{
		for (auto i = boxControlIds.begin(); i != boxControlIds.end(); )
		{
			if (!boxes[*i]->SpawnControlWindow(*i, wnd.Gfx()))
			{
				i = boxControlIds.erase(i);
			}
			else
			{
				i++;
			}
		}
	}

private:
	void Update();
	void ComposeFrame();

private:
	float dt			    = 0.0f;
	float speedF			= 1.0f;
	bool  show_demo_window	= false;

	std::optional<int>	comboBoxIndex;
	std::set<int>		boxControlIds;

private:
	ImguiManager imgui;
	Window wnd;
	FrameTimer timer;
	Camera cam;
	PointLight pl;

	std::vector<std::unique_ptr<Box>>		boxes;
	std::vector<std::unique_ptr<Cylinder>>	cils;
	std::vector<std::unique_ptr<Pyramid>>	pyrms;
	std::vector<std::unique_ptr<BoxTex>>	tboxes;

	static constexpr unsigned int boxCount		= 45;
	static constexpr unsigned int cilCount		= 45;
	static constexpr unsigned int pyrmsCount	= 45;
	static constexpr unsigned int tboxCount		= 45;
};