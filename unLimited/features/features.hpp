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

namespace skinchanger
{
	void apply_skins();
	void fix_icons();

	void unload();
}

namespace visuals
{
	void glow();
	void radar();

	void thirdperson_override_view();
	void thirdperson_fsn();
}

namespace misc
{
	void bhop(CUserCmd* cmd);
	void autostrafe(CUserCmd* cmd);
	void show_ranks(CUserCmd* cmd);
	void nightmode();
	void airstuck(CUserCmd* cmd);
}
