#include "hooks.hpp"

#include "../features/features.hpp"
#include "../math.hpp"
#include "../vmt_hook.hpp"

bool __stdcall hooks::hk_create_move(float sample_input_frametime, CUserCmd* cmd)
{
	static auto o_create_move = client_mode_hook->get_original<bool(__thiscall*)(void*, float, CUserCmd*)>(index::create_move);

	if (!cmd || !cmd->command_number)
		return o_create_move(g_client_mode, sample_input_frametime, cmd);
	
	uintptr_t* frame_ptr;
	__asm mov frame_ptr, ebp

	bool& send_packet = *reinterpret_cast<bool*>(*frame_ptr - 0x1C);

	backtracking::process(cmd);
	aimbot::process(cmd, send_packet);
	misc::bhop(cmd);
	misc::autostrafe(cmd);
	misc::airstuck(cmd);
	misc::show_ranks(cmd);

	// FIXMEW: put in FSN
	visuals::radar();

	// doesn't matter where we call this, as long as it's called somewhat regularly
	misc::nightmode();

	QAngle old_angle = cmd->viewangles;
	float old_forward = cmd->forwardmove;
	float old_side = cmd->sidemove;

	antiaim::process(cmd, send_packet);

	math::correct_movement(cmd, old_angle, old_forward, old_side);

	cmd->viewangles.Clamp();

	//if (!options::aim::silent && (!options::rage::aa || !options::rage::silent))
	//	g_engine->SetViewAngles(cmd->viewangles);

	return false;
}
