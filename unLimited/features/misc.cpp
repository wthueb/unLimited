#include "features.hpp"

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

void misc::nightmode()
{
	static bool old = false;

	// only enable/disable once per setting toggle
	if (options::misc::nightmode == old)
		return;

	static auto r_drawspecificstaticprop = g_cvar->FindVar("r_drawspecificstaticprop");
	static auto sv_skyname = g_cvar->FindVar("sv_skyname");

	r_drawspecificstaticprop->SetValue(options::misc::nightmode ? 0 : 1);

	static std::string old_sky{};

	if (options::misc::nightmode)
		old_sky = sv_skyname->GetString();

	sv_skyname->SetValue(options::misc::nightmode ? "sky_csgo_night02" : old_sky.c_str());

	for (auto i = g_material_system->FirstMaterial(); i != g_material_system->InvalidMaterial(); i = g_material_system->NextMaterial(i))
	{
		IMaterial* mat = g_material_system->GetMaterial(i);
		if (!mat || mat->IsErrorMaterial())
			continue;

		if (strstr(mat->GetTextureGroupName(), "World") || strstr(mat->GetTextureGroupName(), "StaticProp"))
		{
			if (options::misc::nightmode)
				mat->ColorModulate(.25f, .25f, .25f);
			else
				mat->ColorModulate(1.f, 1.f, 1.f);
		}
		else if (strstr(mat->GetTextureGroupName(), "Model"))
		{
			if (options::misc::nightmode)
				mat->ColorModulate(.4f, .4f, .4f);
			else
				mat->ColorModulate(1.f, 1.f, 1.f);
		}
	}

	old = options::misc::nightmode;
}

void misc::radar()
{
	if (!options::esp::radar || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto entity = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!entity)
			continue;

		if (entity == localplayer || entity->IsDormant() || !entity->IsAlive())
			continue;

		entity->GetSpotted() = true;
	}
}

void misc::show_ranks(CUserCmd* cmd)
{
	if (!options::misc::show_ranks)
		return;

	if (!(cmd->buttons & IN_SCORE))
		return;

	static int arg[3] = { 0, 0, 0 };

	using MsgFunc_ServerRankRevealAllFn = bool(__cdecl*)(int[3]);

	static MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll =
		reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(utils::find_signature("client.dll", "55 8B EC 8B 0D ? ? ? ? 68"));

	MsgFunc_ServerRankRevealAll(arg);
}
