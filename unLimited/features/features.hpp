#pragma once

#include "../sdk/sdk.hpp"

namespace aimbot
{
	void process(CUserCmd* cmd, bool& send_packet);
}

namespace antiaim
{
	void process(CUserCmd* cmd, bool& send_packet);
	void draw_angles();
}

namespace backtracking
{
	void process(CUserCmd* cmd);
	void draw();
}

namespace esp
{
	void glow();
	void radar();
}

namespace misc
{
	void bhop(CUserCmd* cmd);
	void autostrafe(CUserCmd* cmd);
	void show_ranks(CUserCmd* cmd);
	void nightmode();
	void airstuck(CUserCmd* cmd);
}
