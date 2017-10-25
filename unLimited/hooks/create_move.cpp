#include "hooks.hpp"

#include "../features/airstuck.hpp"
#include "../features/backtracking.hpp"
#include "../features/legitaim.hpp"

#include "../math.hpp"

bool __stdcall hooks::hk_create_move(float sample_input_frametime, CUserCmd* cmd)
{
	static auto o_create_move = client_mode_hook->get_original<bool(__thiscall*)(void*, float, CUserCmd*)>(index::create_move);

	o_create_move(g_client_mode, sample_input_frametime, cmd);

	uintptr_t* frame_ptr;
	__asm mov frame_ptr, ebp

	auto send_packet = *reinterpret_cast<bool*>(*frame_ptr - 0x1C);

	QAngle old_angle = cmd->viewangles;
	float old_forward = cmd->forwardmove;
	float old_side = cmd->sidemove;

	if (cmd && cmd->command_number)
	{
		airstuck::create_move(cmd);
		backtracking::create_move(cmd);
		legitaim::create_move(cmd, send_packet);
	}

	cmd->viewangles.Clamp();

	math::correct_movement(cmd, old_angle, old_forward, old_side);
	
	return false;
}
