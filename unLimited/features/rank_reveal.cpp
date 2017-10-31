#include "misc.hpp"

#include "../options.hpp"

void misc::show_ranks(CUserCmd* cmd)
{
	if (!options::misc::show_ranks)
		return;

	if (!(cmd->buttons & IN_SCORE))
		return;

	static int arg0[3] = { 0, 0, 0 };

	using MsgFunc_ServerRankRevealAllFn = bool(__cdecl*)(int[3]);

	static MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll =
		reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(utils::find_signature("client.dll", "55 8B EC 8B 0D ? ? ? ? 68"));

	MsgFunc_ServerRankRevealAll(arg0);
}
