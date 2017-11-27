#include "hooks.hpp"

#include "../features/features.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

void __stdcall hooks::hk_frame_stage_notify(ClientFrameStage_t stage)
{
	static auto o_frame_stage_notify = client_hook->get_original<void(__thiscall*)(void*, ClientFrameStage_t)>(index::frame_stage_notify);

	if (stage == FRAME_RENDER_START)
	{
		visuals::thirdperson_fsn();
		visuals::radar();
		visuals::noflash();
		misc::nightmode();
		misc::chat_spam();
	}
	else if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		skinchanger::apply_skins();
	}

	o_frame_stage_notify(g_client, stage);
}
