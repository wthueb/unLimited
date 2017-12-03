#include "features.hpp"

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

void aimbot::process(CUserCmd* _cmd, bool& send_packet)
{
	if (!options::aim::enabled || !g_engine->IsInGame())
		return;

	drop_target();

	localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	active_weapon = static_cast<C_BaseCombatWeapon*>(g_entity_list->GetClientEntityFromHandle(localplayer->GetActiveWeapon()));
	if (!active_weapon)
		return;

	cmd = _cmd;

	if (active_weapon->IsKnife() || active_weapon->IsNade() || active_weapon->IsBomb())
		return;
	
	bool should_correct = false;

	if (options::aim::aimbot)
	{
		static bool found = false; // FIXMEW: just check best_target?

		if (options::aim::on_shoot && cmd->buttons & IN_ATTACK ||
			options::aim::aim_key && GetAsyncKeyState(options::aim::aim_key))
		{
			if (!found || options::aim::reaim)
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

	if (options::aim::rcs && !should_correct &&
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
		old_angle.pitch = old_angle.yaw = old_angle.roll = 0;
	}
}

void find_target()
{
	float best_fov = options::aim::fov;

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto potential = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!potential)
			continue;

		if (!potential->IsValid() || potential == localplayer)
			continue;

		if (!options::aim::friendlies &&
			potential->GetTeam() == localplayer->GetTeam())
			continue;

		if (options::aim::vis_check &&
			(!options::aim::aim_key || !GetAsyncKeyState(options::aim::aim_key)) &&
			!is_visible(potential))
				continue;

		Vector target_pos{ potential->GetBonePos(Bone(options::aim::bone)) };
		Vector eye_pos{ localplayer->GetEyePosition() };
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

	if (!options::aim::friendlies &&
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

	if (options::aim::vis_check &&
		(!options::aim::aim_key || !GetAsyncKeyState(options::aim::aim_key)) &&
		!is_visible(target))
		return;

	Vector eye_pos{ localplayer->GetEyePosition() };
	Vector target_pos{ target->GetBonePos(Bone(options::aim::bone)) };
	Vector relative{ eye_pos - target_pos };

	QAngle dst;
	math::VectorAngles(relative, dst);

	if (math::get_fov(cmd->viewangles + localplayer->GetAimPunch() * 2.f, dst) > options::aim::fov)
		return;

	if (!active_weapon->IsSniper() && !active_weapon->IsPistol() && !active_weapon->IsShotgun())
		dst -= localplayer->GetAimPunch() * 2.f;

	dst.Clamp();

	if (options::aim::smooth)
	{
		QAngle delta{ dst - cmd->viewangles };
		delta.Clamp();
		delta /= std::max(1.f, options::aim::smooth_amount);
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
	Vector end{ player->GetBonePos(Bone(options::aim::bone)) };

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
