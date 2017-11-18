#pragma once

#include "../sdk/sdk.hpp"

namespace aimbot
{
	void create_move(CUserCmd* cmd, bool& send_packet);
}

namespace backtracking
{
	void create_move(CUserCmd* cmd);
	void paint_traverse();
}

namespace esp
{
	void paint_traverse();
	void do_post_screen_space_effects();
}

namespace misc
{
	void airstuck(CUserCmd* cmd);
	void nightmode();
	void radar();
	void show_ranks(CUserCmd* cmd);
}
