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
	void unload();

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

	namespace antiaim
	{
		enum aa_type
		{
			LEGIT,
			RAGE,
			LBY_SIDEWAYS,
			SPIN_SLOW,
			SPIN_FAST,
			AA_COUNT
		};

		extern bool enabled;
		extern bool text;

		extern int type;
		extern bool fakelag;
	}

	namespace visuals
	{
		extern bool enabled;

		extern bool glow;
		extern float glow_alpha;
		extern int glow_style;

		extern bool players;
		extern bool friendlies;
		extern bool chickens;
		extern bool defuse_kits;
		extern bool c4;
		extern bool weapons;

		extern bool radar;

		extern bool thirdperson;
		extern int thirdperson_offset;
	}

	namespace misc
	{
		extern bool bhop;
		extern bool autostrafe;

		extern bool backtracking;
		extern bool backtracking_vis;
		extern int backtracking_amt;

		extern bool show_ranks;

		extern bool nightmode;

		extern bool airstuck;
		extern int airstuck_key;

		extern float fov;
	}
}
