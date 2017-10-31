#include "misc.hpp"

#include "../options.hpp"

void misc::airstuck(CUserCmd* cmd)
{
	if (!options::misc::airstuck)
		return;

	if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
		return;

	if (options::misc::airstuck_key && GetAsyncKeyState(options::misc::airstuck_key))
	{
		cmd->tick_count = 16777216;
	}
}
