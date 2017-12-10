#include "features.hpp"

#include <TimeAPI.h>

#include <algorithm>

#include "../math.hpp"
#include "../options.hpp"

void rcs();

void find_target();
void drop_target();
void correct_aim();

bool is_visible(C_BasePlayer* player);

static C_BasePlayer* localplayer = nullptr;
static C_BaseCombatWeapon* active_weapon = nullptr;
static CUserCmd* cmd = nullptr;
static int best_target = -1;
static Bone bone = BONE_INVALID;

void aimbot::process(CUserCmd* _cmd, bool& send_packet)
{
	if (!options.aim_enabled || !g_engine->IsInGame())
		return;

	localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	active_weapon = static_cast<C_BaseCombatWeapon*>(g_entity_list->GetClientEntityFromHandle(localplayer->GetActiveWeapon()));
	if (!active_weapon)
		return;

	if (active_weapon->IsKnife() || active_weapon->IsNade() || active_weapon->IsBomb())
		return;

	drop_target();
	
	cmd = _cmd;

	bool should_correct = false;

	if (options.aim_aimbot)
	{
		static bool found = false; // FIXMEW: just check best_target?

		if (options.aim_on_shoot && cmd->buttons & IN_ATTACK ||
			options.aim_aim_key && GetAsyncKeyState(options.aim_aim_key))
		{
			if (!found || options.aim_reaim)
			{
				find_target();
				found = true;
			}

			should_correct = true;
		}
		else
			found = false;

		if (should_correct)
			correct_aim();
	}

	if (options.aim_rcs && !should_correct &&
		localplayer->GetShotsFired() > 1)
		rcs();

	g_engine->SetViewAngles(cmd->viewangles);
}

void rcs()
{
	static QAngle old_angle{};

	if (cmd->buttons & IN_ATTACK)
	{
		QAngle aim_punch{ localplayer->GetAimPunch() * 2.f };

		QAngle dst{ cmd->viewangles + old_angle - aim_punch };

		dst.Clamp();

		cmd->viewangles = dst;

		old_angle = aim_punch;
	}
	else
	{
		old_angle.Init();
	}
}

void find_target()
{
	float best_fov = options.aim_fov;
	
	Vector eye_pos{ localplayer->GetEyePosition() };

	if (options.aim_bone == -1)
		bone = BONE_MIDDLE_SPINAL_COLUMN;
	else
		bone = Bone(options.aim_bone);

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto potential = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!potential)
			continue;

		if (!potential->IsValid() || potential == localplayer)
			continue;

		if (!options.aim_friendlies &&
			potential->GetTeam() == localplayer->GetTeam())
			continue;

		if (options.aim_vis_check &&
			(!options.aim_aim_key || !GetAsyncKeyState(options.aim_aim_key)) &&
			!is_visible(potential))
				continue;

		Vector target_pos{ potential->GetBonePos(bone) };
		Vector relative{ eye_pos - target_pos };

		QAngle angle;
		math::VectorAngles(relative, angle);

		auto fov = math::get_fov(cmd->viewangles + localplayer->GetAimPunch() * 2.f, angle);

		if (fov <= best_fov)
		{
			best_fov = fov;
			best_target = i;
		}
	}

	if (best_target != -1 && options.aim_bone == -1)
	{
		auto target = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(best_target));
		if (!target)
			return;

		for (auto it : options.bones)
		{
			auto potential = it.first;

			Vector target_pos{ target->GetBonePos(potential) };
			Vector relative{ eye_pos - target_pos };

			QAngle angle;
			math::VectorAngles(relative, angle);

			auto fov = math::get_fov(cmd->viewangles + localplayer->GetAimPunch() * 2.f, angle);

			if (fov <= best_fov)
			{
				best_fov = fov;
				bone = potential;
			}
		}
	}
}

void drop_target()
{
	if (best_target == -1)
		return;

	auto target = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(best_target));
	if (!target)
		return;

	if (!target->IsValid() || target == localplayer)
		best_target = -1;

	if (!options.aim_friendlies &&
		target->GetTeam() == localplayer->GetTeam())
		best_target = -1;
}

void correct_aim()
{
	if (best_target == -1)
		return;

	auto target = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(best_target));
	if (!target)
		return;

	if (!target->IsValid() || target == localplayer)
		return;

	if (options.aim_vis_check &&
		(!options.aim_aim_key || !GetAsyncKeyState(options.aim_aim_key)) &&
		!is_visible(target))
		return;

	Vector eye_pos{ localplayer->GetEyePosition() };
	Vector target_pos{ target->GetBonePos(bone) };
	Vector relative{ eye_pos - target_pos };

	QAngle dst;
	math::VectorAngles(relative, dst);

	if (math::get_fov(cmd->viewangles + localplayer->GetAimPunch() * 2.f, dst) > options.aim_fov)
		return;

	if (localplayer->GetShotsFired() > 1)
		dst -= localplayer->GetAimPunch() * 2.f;
	
	dst.Clamp();

	if (options.aim_smooth)
	{
		QAngle delta{ dst - cmd->viewangles };

		delta.Clamp();
		
		delta /= std::max(1.f, options.aim_smooth_amount);
		
		dst = cmd->viewangles + delta;
	}

	dst.Clamp();

	cmd->viewangles = dst;
}

bool is_visible(C_BasePlayer* player)
{
	using LineGoesThroughSmokeFn = bool(__cdecl*)(Vector, Vector, bool);

	static LineGoesThroughSmokeFn LineGoesThroughSmoke =
		reinterpret_cast<LineGoesThroughSmokeFn>(utils::find_signature("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

	Vector start{ localplayer->GetEyePosition() };
	Vector end{ player->GetBonePos(bone) };

	if (LineGoesThroughSmoke(start, end, true))
		return false;

	Ray_t ray{};
	ray.Init(start, end);

	CTraceFilter filter{};
	filter.pSkip = localplayer;

	trace_t tr;
	g_engine_trace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr);

	return tr.m_pEnt == player;
}
