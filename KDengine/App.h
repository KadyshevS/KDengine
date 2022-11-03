#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Box.h"

class App
{
	Window wnd;
	FrameTimer timer;

	std::vector<std::unique_ptr<class Box>> boxes;
	static constexpr size_t boxCount = 80;

	float dt;

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

			wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
			ComposeFrame();
			wnd.Gfx().EndFrame();
		}
	}
	~App() = default;

private:
	void Update();
	void ComposeFrame();
};