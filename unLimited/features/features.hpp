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
	void fix_icons(IGameEvent* event);
	void fix_anims();
}

namespace visuals
{
	void chams();
	void glow();
	void skeletons();

	void radar();

	void thirdperson_override_view();
	void thirdperson_fsn();

	void noflash();
}

namespace misc
{
	void bhop(CUserCmd* cmd);
	void autostrafe(CUserCmd* cmd);
	void show_ranks(CUserCmd* cmd);
	void nightmode();
	void airstuck(CUserCmd* cmd);
	void chat_spam();
}
