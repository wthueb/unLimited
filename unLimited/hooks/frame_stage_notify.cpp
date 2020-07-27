#include "hooks.hpp"

#include "../features/features.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

void __stdcall hooks::hk_frame_stage_notify(ClientFrameStage stage)
{
    static const auto o_frame_stage_notify = client_hook->get_original<void(__thiscall*)(void*, ClientFrameStage)>(index::frame_stage_notify);

    if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
    {
        skinchanger::apply_skins();
        skinchanger::fix_anims();
    }
    else if (stage == FRAME_RENDER_START)
    {
        visuals::thirdperson_fsn();
        visuals::radar();
        visuals::noflash();
        misc::nightmode();
        misc::chat_spam();
        misc::callout();
    }

    o_frame_stage_notify(g_client, stage);
}
