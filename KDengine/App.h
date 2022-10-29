#pragma once
#include "Window.h"
#include "FrameTimer.h"

class App
{
	Window wnd;
	FrameTimer timer;

public:
	App();
	int Go();

private:
	void ComposeFrame();
};