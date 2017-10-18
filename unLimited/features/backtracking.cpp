#include "backtracking.hpp"

struct backtrack_data
{
	float simtime;
	Vector hitbox_pos;
};

backtrack_data head_positions[64][12];

void backtracking::create_move(CUserCmd* cmd)
{
	//if (options::misc::backtracking)
	{
		auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
		if (!localplayer)
			return;

		if (!localplayer->IsAlive())
			return;

		int best_target = -1;
		float best_fov = FLT_MAX;

		for (auto i = 1; i < g_engine->GetMaxClients(); ++i)
		{
			auto entity = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
			if (!entity)
				continue;

			if (entity == localplayer || entity->IsDormant() ||
				!entity->IsAlive() || entity->GetTeam() == localplayer->GetTeam())
				continue;

			float simtime = entity->GetSimulationTime();
			Vector hitbox_pos = GetHitboxPosition(entity, 0);

			head_positions[i][cmd->command_number % 13] = backtrack_data{ simtime, hitbox_pos };
			Vector view_dir = AngleVector(cmd->viewangles + localplayer->GetAimPunch() * 2.f);
			float fov = distance_point_to_line(hitbox_pos, localplayer->GetEyePosition(), view_dir);

			if (best_fov > fov)
			{
				best_fov = fov;
				best_target = i;
			}
		}

		float best_target_simtime;

		if (best_target != -1)
		{
			float temp = FLT_MAX;
			Vector view_dir = AngleVector(cmd->viewangles + localplayer->GetAimPunch() * 2.f);
			for (auto i = 0; i < 12; ++i)
			{
				float temp_fov = distance_point_to_line(head_positions[best_target][i].hitbox_pos, localplayer->GetEyePosition(), view_dir);
				if (temp > temp_fov && head_positions[best_target][i].simtime > localplayer->GetSimulationTime() - 1)
				{
					temp = temp_fov;
					best_target_simtime = head_positions[best_target][i].simtime;
				}
			}
		}
	}
}
