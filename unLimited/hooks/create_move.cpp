#include "hooks.hpp"

#include "../features/aimbot.hpp"
#include "../features/airstuck.hpp"
#include "../features/backtracking.hpp"
#include "../features/misc.hpp"

#include "../math.hpp"

bool __stdcall hooks::hk_create_move(float sample_input_frametime, CUserCmd* cmd)
{
	if (!cmd || !cmd->command_number)
		return true;

	uintptr_t* frame_ptr;
	__asm mov frame_ptr, ebp

	auto send_packet = *reinterpret_cast<bool*>(*frame_ptr - 0x1C);

	QAngle old_angle = cmd->viewangles;
	float old_forward = cmd->forwardmove;
	float old_side = cmd->sidemove;

	aimbot::create_move(cmd, send_packet);
	airstuck::create_move(cmd);
	backtracking::create_move(cmd);
	misc::nightmode(); // doesn't matter where we call this

	cmd->viewangles.Clamp();

	math::correct_movement(cmd, old_angle, old_forward, old_side);

	//if (!options::aim::silent && (!options::rage::aa || !options::rage::silent))
		g_engine->SetViewAngles(cmd->viewangles);

	return false;
}
