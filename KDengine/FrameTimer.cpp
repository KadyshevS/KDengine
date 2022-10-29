#include "FrameTimer.h"
using namespace std::chrono;

FrameTimer::FrameTimer() noexcept
{
	last = steady_clock::now();
}

float FrameTimer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frametime = last - old;
	return frametime.count();
}
float FrameTimer::Peek() const noexcept
{
	return duration<float>( steady_clock::now() - last ).count();
}
