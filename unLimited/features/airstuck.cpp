#include "airstuck.hpp"

#include "../options.hpp"

void airstuck::create_move(CUserCmd* cmd)
{
	if (!options::misc::airstuck)
		return;

	if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
		return;

	if (GetAsyncKeyState(options::misc::airstuck_key))
	{
		cmd->tick_count = 16777216;
	}
}
