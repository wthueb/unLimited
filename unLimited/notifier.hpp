#pragma once

#include <Windows.h>

#include <string>
#include <vector>

#include "options.hpp"
#include "sdk/sdk.hpp"
#include "utils.hpp"

class Callback
{
public:
    virtual void Run(HTTPRequestCompleted_t* pvParam);
    virtual void Run(HTTPRequestCompleted_t* pvParam, bool bIOFailure, SteamAPICall_t hSteamAPICall) { Run(pvParam); };
    virtual int GetCallbackSizeBytes() { return sizeof(HTTPRequestCompleted_t); }

protected:
    uint8_t m_nCallbackFlags = 0;
    int m_iCallback = HTTPRequestCompleted_t::k_iCallback;
};

static void SteamAPI_RegisterCallResult(Callback* callback, SteamAPICall_t handle)
{
    static const auto func = reinterpret_cast<void(__cdecl*)(Callback*, SteamAPICall_t)>(GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamAPI_RegisterCallResult"));
    
    func(callback, handle);
}

template<typename T>
void add_opt(const HTTPRequestHandle& handle, const char* name, T value)
{
    g_steam_http->SetHTTPRequestGetOrPostParameter(handle, name, std::to_string(value).c_str());
};

static Callback g_check_callback{};

namespace notifier
{
    template<typename T>
    static void option_changed(const char* name, T value)
    {
        const auto handle = g_steam_http->CreateHTTPRequest(HTTPMethod::POST, "http://wi1.xyz/api/option-changed.php");

        const auto steam_name = g_steam_friends->GetPersonaName();
        const auto account_id = std::to_string(g_steam_user->GetSteamID().account_id).c_str();

        g_steam_http->SetHTTPRequestGetOrPostParameter(handle, "steam_name", steam_name);
        g_steam_http->SetHTTPRequestGetOrPostParameter(handle, "account_id", account_id);
        
        g_steam_http->SetHTTPRequestGetOrPostParameter(handle, name, std::to_string(value).c_str());

        SteamAPICall_t api_call;
        g_steam_http->SendHTTPRequest(handle, &api_call);

        utils::printf("option changed\n");

        SteamAPI_RegisterCallResult(&g_check_callback, api_call);
    }

    static void update_all_options()
    {
        const auto handle = g_steam_http->CreateHTTPRequest(HTTPMethod::POST, "http://wi1.xyz/api/option-changed.php");

        const auto steam_name = g_steam_friends->GetPersonaName();
        const auto account_id = std::to_string(g_steam_user->GetSteamID().account_id).c_str();

        g_steam_http->SetHTTPRequestGetOrPostParameter(handle, "steam_name", steam_name);
        g_steam_http->SetHTTPRequestGetOrPostParameter(handle, "account_id", account_id);

        // unholy amount of repeated code there must be a better way to do this
        {
            // aim

            add_opt(handle, "aim", options.aim_enabled);

            add_opt(handle, "aimbot", options.aim_aimbot);
            add_opt(handle, "fov", options.aim_fov);
            add_opt(handle, "smooth", options.aim_smooth);
            add_opt(handle, "smooth amount", options.aim_smooth_amount);
            add_opt(handle, "bone", options.aim_bone);
            add_opt(handle, "on shoot", options.aim_on_shoot);
            add_opt(handle, "reaim", options.aim_reaim);
            add_opt(handle, "aim key", options.aim_aim_key);
            add_opt(handle, "shoot teammates", options.aim_friendlies);
            add_opt(handle, "visible check", options.aim_vis_check);

            add_opt(handle, "rcs", options.aim_rcs);

            // anti-aim

            add_opt(handle, "anti-aim", options.aa_enabled); bool aa_enabled = false;
            add_opt(handle, "show angles/choke", options.aa_show); bool aa_show = false;

            add_opt(handle, "aa type", int(options.aa_type));
            add_opt(handle, "fakelag", options.misc_fakelag);

            // visuals

            add_opt(handle, "visuals", options.visuals_enabled);

            add_opt(handle, "chams", options.visuals_chams);
            add_opt(handle, "ignorez (through walls)", options.visuals_chams_ignorez);
            add_opt(handle, "flat", options.visuals_chams_flat);
            add_opt(handle, "wireframe", options.visuals_chams_wireframe);
            add_opt(handle, "glass", options.visuals_chams_glass);

            add_opt(handle, "arms", options.visuals_chams_arms);
            add_opt(handle, "no hands", options.visuals_chams_no_hands);

            add_opt(handle, "glow", options.visuals_glow);
            add_opt(handle, "glow alpha", options.visuals_glow_alpha);
            add_opt(handle, "glow style", options.visuals_glow_style);

            add_opt(handle, "skeletons", options.visuals_skeletons);

            add_opt(handle, "players", options.visuals_players);
            add_opt(handle, "friendlies", options.visuals_friendlies);
            add_opt(handle, "chickens", options.visuals_chickens);
            add_opt(handle, "defuse kits", options.visuals_defuse_kits);
            add_opt(handle, "c4", options.visuals_c4);
            add_opt(handle, "weapons", options.visuals_weapons);

            add_opt(handle, "radar", options.visuals_radar);

            add_opt(handle, "thirdperson", options.visuals_thirdperson);
            add_opt(handle, "thirdperson offset", options.visuals_thirdperson_offset);

            add_opt(handle, "disable scope", options.visuals_noscope);

            add_opt(handle, "noflash", options.visuals_noflash);

            // misc

            add_opt(handle, "bhop", options.misc_bhop);
            add_opt(handle, "autostrafe", options.misc_autostrafe);

            add_opt(handle, "backtracking", options.misc_backtracking);
            add_opt(handle, "backtracking visual", options.misc_backtracking_vis);
            add_opt(handle, "backtracking amount (ticks, max 12)", options.misc_backtracking_amt);

            add_opt(handle, "show ranks", options.misc_show_ranks);

            add_opt(handle, "nightmode", options.misc_nightmode);

            add_opt(handle, "airstuck", options.misc_airstuck);
            add_opt(handle, "airstuck key", options.misc_airstuck_key);

            add_opt(handle, "view fov", options.misc_fov);
            add_opt(handle, "fov while scoped", options.misc_fov_scoped);

            add_opt(handle, "chat spam", options.misc_chat_spam);
        }

        SteamAPICall_t api_call;
        g_steam_http->SendHTTPRequest(handle, &api_call);

        utils::printf("all options updated\n");

        SteamAPI_RegisterCallResult(&g_check_callback, api_call);
    }
}