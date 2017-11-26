#include "features.hpp"

#include "../options.hpp"

#include "../math.hpp"

#define TIME_TO_TICKS(dt)  (int(.5f + float(dt) / g_global_vars->interval_per_tick))

struct backtrack_data
{
	float simtime;
	Vector headpos;
};

backtrack_data head_positions[64][12]{};

void backtracking::process(CUserCmd* cmd)
{
	if (!options::misc::backtracking)
		return;
	
	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	if (!localplayer->IsAlive())
		return;

	int best_target = -1;
	float best_fov = FLT_MAX;

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!player)
			continue;

		if (!player->IsValid() || player == localplayer || player->GetTeam() == localplayer->GetTeam())
			continue;

		float simtime = player->GetSimulationTime();
		Vector headpos{ player->GetBonePos(BONE_HEAD) };

		head_positions[i][cmd->command_number % 13] = backtrack_data{ simtime, headpos };

		Vector viewdir;
		math::AngleVectors(cmd->viewangles + localplayer->GetAimPunch() * 2.f, &viewdir);

		float fov = math::distance_point_to_line(headpos, localplayer->GetEyePosition(), viewdir);

		if (best_fov > fov)
		{
			best_fov = fov;
			best_target = i;
		}
	}

	if (best_target != -1)
	{
		float best_dist = FLT_MAX;
		float best_target_simtime;

		Vector view_forward;
		math::AngleVectors(cmd->viewangles + localplayer->GetAimPunch() * 2.f, &view_forward);

		for (auto i = 0; i < options::misc::backtracking_amt; ++i)
		{
			float dist = math::distance_point_to_line(head_positions[best_target][i].headpos, localplayer->GetEyePosition(), view_forward);

			if (dist < best_dist && head_positions[best_target][i].simtime > localplayer->GetSimulationTime() - 1)
			{
				best_dist = dist;
				best_target_simtime = head_positions[best_target][i].simtime;
			}
		}

		if (cmd->buttons & IN_ATTACK && best_dist != FLT_MAX)
			cmd->tick_count = TIME_TO_TICKS(best_target_simtime);
	}
}

void backtracking::draw()
{
	if (!options::misc::backtracking || !options::misc::backtracking_vis)
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!player)
			continue;

		if (!player->IsValid() || player == localplayer || player->GetTeam() == localplayer->GetTeam())
			continue;

		for (auto t = 0; t < options::misc::backtracking_amt; ++t)
		{
			if (head_positions[i][t].simtime > localplayer->GetSimulationTime() - 1.f)
			{
				Vector out;
				if (!g_debug_overlay->ScreenPosition(head_positions[i][t].headpos, out))
				{
					g_surface->DrawSetColor(Color{ 255, 0, 0 });
					g_surface->DrawOutlinedRect(int(out.x), int(out.y), int(out.x + 2.f), int(out.y + 2.f));
				}
			}
		}
	}
}
