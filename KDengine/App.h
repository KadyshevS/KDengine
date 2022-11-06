#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Drawable.h"

class App
{
	Window wnd;
	FrameTimer timer;

	float dt;

	std::unique_ptr<Drawable> box;

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

			wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.3f);
			ComposeFrame();
			wnd.Gfx().EndFrame();
		}
	}
	~App() = default;

private:
	void Update();
	void ComposeFrame();
};