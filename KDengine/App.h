#pragma once
#include "Window.h"
#include "FrameTimer.h"

class App
{
	Window wnd;
	FrameTimer timer;

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

private:
	void Update();
	void ComposeFrame();
};