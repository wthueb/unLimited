#include "features.hpp"

#include <chrono>

#include "../options.hpp"

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

void misc::show_ranks(CUserCmd* cmd)
{
	if (!options.misc_show_ranks || !g_engine->IsInGame())
		return;

	if (!(cmd->buttons & IN_SCORE))
		return;

	static int arg[3] = { 0, 0, 0 };

	using MsgFunc_ServerRankRevealAllFn = bool(__cdecl*)(int[3]);

	static MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll =
		reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(utils::find_signature("client.dll", "55 8B EC 8B 0D ? ? ? ? 68"));

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
	if (!options.misc_chat_spam || !g_engine->IsInGame())
		return;

	static std::chrono::time_point<std::chrono::steady_clock> last_time{};

	auto cur_time = std::chrono::steady_clock::now();

	auto duration_since_last = std::chrono::duration_cast<std::chrono::duration<double>>(cur_time - last_time);

	// spam every .35 seconds
	if (duration_since_last.count() < .35)
		return;

	g_engine->ClientCmd_Unrestricted("say wi1.xyz takes ahold of me");

	last_time = std::chrono::steady_clock::now();
}
