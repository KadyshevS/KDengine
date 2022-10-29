#pragma once
#include <chrono>

class FrameTimer
{
	std::chrono::steady_clock::time_point last;

public:
	FrameTimer() noexcept;

	float Mark() noexcept;
	float Peek() const noexcept;
};