#include "options.hpp"

#include <Windows.h>

namespace options
{
	void init()
	{
		// FIXMEW: config
	}

	namespace misc
	{
		bool backtracking = true;
		bool backtracking_vis = true;

		bool airstuck = true;
		int airstuck_key = VK_END;
	}
}
