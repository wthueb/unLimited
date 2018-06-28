#pragma once

#include "../sdk/sdk.hpp"

using DrawModelExecuteFn = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

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
	void chams2(DrawModelExecuteFn o_draw_model_execute, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix);
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
	void callout();
}
