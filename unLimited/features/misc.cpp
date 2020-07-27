#include "features.hpp"

#include <chrono>
#include <forward_list>
#include <sstream>

#include "../options.hpp"

bool g_choking = false;
int g_choked_ticks = 0;

void misc::bhop(CUserCmd* cmd)
{
    if (!options.misc_bhop || !g_engine->IsInGame())
        return;

    auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
    if (!localplayer)
        return;

    if (cmd->buttons & IN_JUMP && !(localplayer->GetFlags() & FL_ONGROUND))
    {
        // release the jump button. whenever you're not in the air (on the ground), jump button gets pressed
        cmd->buttons &= ~IN_JUMP;
    }
}

void misc::autostrafe(CUserCmd* cmd)
{
    if (!options.misc_autostrafe || !g_engine->IsInGame())
        return;

    auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
    if (!localplayer)
        return;

    if (localplayer->GetFlags() & FL_ONGROUND)
        return;

    if (cmd->mousedy > 1)
        cmd->sidemove = 450.f;
    else if (cmd->mousedy < -1)
        cmd->sidemove = -450.f;
    else
        cmd->sidemove = cmd->command_number % 2 ? 450.f : -450.f;
}

void misc::fakelag(CUserCmd* cmd, bool& send_packet)
{
    if (!options.misc_fakelag || !g_engine->IsInGame())
        return;
    
    auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
    if (!localplayer)
        return;

    if (!localplayer->IsAlive())
        return;

    auto weapon = localplayer->GetActiveWeapon().Get();
    if (!weapon)
        return;

    // if not trying to shoot or jump
    if (cmd->buttons & IN_ATTACK &&
        weapon->GetNextPrimaryAttack() <= g_global_vars->get_realtime(cmd) &&
        weapon->GetAmmo() > 0 ||
        !(localplayer->GetFlags() & FL_ONGROUND && cmd->buttons & IN_JUMP))
    {
        if (++g_choked_ticks < 15)
        {
            send_packet = false;
        }
        else
        {
            send_packet = true;
        }
    }
    else
    {
        send_packet = true; // should already be true, but semantics

        if (options.aa_enabled)
        {
            if (options.aa_type == aa_type::SPIN_SLOW && options.aa_type == aa_type::SPIN_FAST)
                send_packet = true;
            else
            {
                static bool flip = false;
                flip = !flip;

                send_packet = flip;
            }
        }
    }

    g_choking = !send_packet;

    if (!send_packet)
        ++g_choked_ticks;
    else
        g_choked_ticks = 0;
}

void misc::show_ranks(CUserCmd* cmd)
{
    if (!options.misc_show_ranks || !g_engine->IsInGame())
        return;

    if (!(cmd->buttons & IN_SCORE))
        return;

    static int arg[3] = { 0, 0, 0 };

    using MsgFunc_ServerRankRevealAllFn = bool(__cdecl*)(int[3]);

    static MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll =
        reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(utils::find_signature("client_panorama.dll", "55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D"));

    MsgFunc_ServerRankRevealAll(arg);
}

void misc::nightmode()
{
    static bool old = false;

    // only enable/disable once per setting toggle
    if (options.misc_nightmode == old || !g_engine->IsInGame())
        return;

    static auto r_drawspecificstaticprop = g_cvar->FindVar("r_drawspecificstaticprop");
    static auto sv_skyname = g_cvar->FindVar("sv_skyname");

    r_drawspecificstaticprop->SetValue(options.misc_nightmode ? 0 : 1);

    static std::string old_sky{};

    if (options.misc_nightmode)
        old_sky = sv_skyname->GetString();

    // FIXMEW: use LoadNamedSky (string ref: "skybox/%s%s" in ida) so it works on non-local servers
    // (spoofing sv_cheats is for losers)
    sv_skyname->SetValue(options.misc_nightmode ? "sky_csgo_night02" : old_sky.c_str());

    for (auto i = g_material_system->FirstMaterial(); i != g_material_system->InvalidMaterial(); i = g_material_system->NextMaterial(i))
    {
        IMaterial* mat = g_material_system->GetMaterial(i);
        if (!mat || mat->IsErrorMaterial())
            continue;

        if (strstr(mat->GetTextureGroupName(), "World") || strstr(mat->GetTextureGroupName(), "StaticProp"))
        {
            if (options.misc_nightmode)
                mat->ColorModulate(.25f, .25f, .25f);
            else
                mat->ColorModulate(1.f, 1.f, 1.f);
        }
        else if (strstr(mat->GetTextureGroupName(), "Model"))
        {
            if (options.misc_nightmode)
                mat->ColorModulate(.4f, .4f, .4f);
            else
                mat->ColorModulate(1.f, 1.f, 1.f);
        }
    }

    old = options.misc_nightmode;
}

void misc::airstuck(CUserCmd* cmd)
{
    if (!options.misc_airstuck || !g_engine->IsInGame())
        return;

    if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
        return;

    if (options.misc_airstuck_key && GetAsyncKeyState(options.misc_airstuck_key))
        cmd->tick_count = 16777216;
}

void misc::chat_spam()
{
    if (!options.misc_chat_spam || options.misc_callout || !g_engine->IsInGame())
        return;

    static std::chrono::time_point<std::chrono::steady_clock> last_time{};

    const auto cur_time = std::chrono::steady_clock::now();

    const auto duration_since_last = std::chrono::duration_cast<std::chrono::duration<double>>(cur_time - last_time);

    // spam every .35 seconds
    if (duration_since_last.count() < .35)
        return;

    g_engine->ClientCmd_Unrestricted("say wi1.xyz takes ahold of me");

    last_time = std::chrono::steady_clock::now();
}

void misc::callout()
{
    if (!options.misc_callout || !g_engine->IsInGame())
        return;

    static std::chrono::time_point<std::chrono::steady_clock> last_time{};

    const auto cur_time = std::chrono::steady_clock::now();

    const auto duration_since_last = std::chrono::duration_cast<std::chrono::duration<double>>(cur_time - last_time);

    // every 5 seconds
    if (duration_since_last.count() < 3)
        return;

    const auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
    if (!localplayer)
        return;

    std::forward_list<std::string> locations;

    for (auto i = 0; i <= g_engine->GetMaxClients(); ++i)
    {
        const auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
        if (!player || !player->IsValid())
            continue;

        if (player->GetTeam() != localplayer->GetTeam())
            continue;

        if (!options.misc_callout_self && player == localplayer)
            continue;

        auto location_key = player->GetLocation();

        if (location_key)
        {
            const auto unicode_location = g_localize->Find(location_key);

            /*const auto format_str = g_localize->Find("#Voice_UseLocation");

            if (format_str)
            {
                player_info_t player_info;
                g_engine->GetPlayerInfo(i, &player_info);

                wchar_t unicode_name[32];
                g_localize->ConvertANSIToUnicode(player_info.name, unicode_name, sizeof(unicode_name));

                wchar_t converted[64];
                g_localize->ConstructString(converted, sizeof(converted), format_str, 2, unicode_name, unicode_location);

                locations.emplace_front(converted);
            }*/

            char location[MAX_PLACE_NAME_LENGTH];
            g_localize->ConvertUnicodeToANSI(unicode_location, location, sizeof(location));

            locations.emplace_front(location);
        }
    }

    if (locations.empty())
        return;

    std::ostringstream callouts;

    const char* sep = "";

    for (auto it = locations.begin(); it != locations.end(); ++it)
    {
        callouts << sep << *it;
        
        sep = ", ";
    }

    auto msg = callouts.str();

    // max chat msg count (126) - len("teammate locations: ")
    msg.resize(106);

    msg = "say teammate locations: " + msg;

    g_engine->ClientCmd_Unrestricted(msg.c_str());

    last_time = std::chrono::steady_clock::now();
}
