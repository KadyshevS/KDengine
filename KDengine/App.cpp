#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(800, 600, "KDEngine App")
{}

void App::Update()
{
}

void App::ComposeFrame()
{
	wnd.Gfx().DrawTestCube(
		-timer.Peek(),
		0.0f, 0.0f
	);
	wnd.Gfx().DrawTestCube(
		timer.Peek(), 
		wnd.mouse.GetPosX() / 400.0f - 1.0f, 
		-wnd.mouse.GetPosY() / 300.f + 1.0f  
	);
}
