#pragma once

#include <vector>

struct option_t
{
	int num;
	const char* name;
};

namespace options
{
	extern std::vector<option_t> bones;
	extern std::vector<option_t> keys;

	void init();

	namespace aim
	{
		extern bool enabled;
		
		extern bool aimbot;
		extern float fov;
		extern bool smooth;
		extern float smooth_amount;
		extern int bone;
		extern bool on_shoot;
		extern bool reaim;
		extern int aim_key;
		extern bool friendlies;
		extern bool visible_only;

		extern bool rcs;
	}

	namespace misc
	{
		extern bool backtracking;
		extern bool backtracking_vis;

		extern bool airstuck;
		extern int airstuck_key;

		extern float fov;

		extern bool nightmode;
	}
}
