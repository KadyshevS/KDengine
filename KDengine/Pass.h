#pragma once
#include "Graphics.h"
#include "Job.h"
#include <vector>

class Pass
{
public:
	void Accept( Job job ) noexcept
	{
		jobs.push_back( job );
	}
	void Execute( Graphics& gfx ) const noexcept(!IS_DEBUG)
	{
		for( const auto& j : jobs )
		{
			j.Execute( gfx );
		}
	}
	void Reset() noexcept
	{
		jobs.clear();
	}
private:
	std::vector<Job> jobs;
};
