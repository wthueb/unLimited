#include "features.hpp"

#include "../options.hpp"

#include "../engine_drawing.hpp"

#define TICKS_TO_TIME(t)  (float(t) * g_global_vars->interval_per_tick)
#define TIME_TO_TICKS(dt) (int(.5f + float(dt) / g_global_vars->interval_per_tick))

static QAngle fake{};
static QAngle real{};

static bool choking = false;
static int choked_ticks = 0;

void antiaim::process(CUserCmd* cmd, bool& send_packet)
{
	if (!options::antiaim::enabled || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	if (!localplayer->IsAlive())
		return;

	auto weapon = localplayer->GetActiveWeapon().Get();
	if (!weapon)
		return;

	if (weapon->IsKnife() || weapon->IsBomb())
		return;

	if (cmd->buttons & IN_ATTACK &&
		g_global_vars->get_realtime(cmd) >= weapon->GetNextPrimaryAttack() &&
		weapon->GetAmmo() > 0)
		return;

	if (cmd->buttons & IN_USE)
		return;

	//if (localplayer->GetMoveType() == MOVETYPE_LADDER)
	//	return;


	if (options::antiaim::fakelag)
	{
		if (choked_ticks < 15)
		{
			send_packet = false;
			++choked_ticks;
		}
		else
		{
			send_packet = true;
			choked_ticks = 0;
		}

		choking = !send_packet;
	}
	else if (options::antiaim::type != options::antiaim::SPIN_SLOW && options::antiaim::type != options::antiaim::SPIN_FAST)
	{
		static bool flip = false;
		flip = !flip;

		send_packet = flip;
	}
	else
		send_packet = true;

	switch (options::antiaim::type)
	{
	case options::antiaim::LEGIT:
	{
		static bool swap = false;

		if (!send_packet)
		{
			if (swap)
				cmd->viewangles.yaw += 90.f;
			else
				cmd->viewangles.yaw -= 90.f;

			swap = !swap;
		}

		break;
	}

	case options::antiaim::RAGE: // fake sideways, emotion; real backwards, emotion
	{
		if (send_packet)
			cmd->viewangles.yaw += 90.f;
		else
			cmd->viewangles.yaw += 180.f;

		cmd->viewangles.pitch = 89.f;

		break;
	}

	case options::antiaim::LBY_SIDEWAYS: // fake backwards, emotion; real sideways opposite lby, emotion
	{
		static float old_lby = 0.f;
		static bool swap = false;

		float lby = localplayer->GetLBY();

		if (lby != old_lby)
		{
			swap = !swap;
			old_lby = lby;
		}

		if (send_packet)
		{
			cmd->viewangles.yaw += 180.f;
		}
		else
		{
			if (swap)
				cmd->viewangles.yaw += 90.f;
			else
				cmd->viewangles.yaw -= 90.f;
		}

		cmd->viewangles.pitch = 89.f;

		break;
	}

	case options::antiaim::SPIN_SLOW:
	{
		float curtime = g_global_vars->get_realtime(cmd);

		cmd->viewangles.yaw = fmodf(curtime * 300.f, 360.f);

		cmd->viewangles.pitch = 89.f;

		break;
	}

	case options::antiaim::SPIN_FAST:
	{
		float curtime = g_global_vars->get_realtime(cmd);

		cmd->viewangles.yaw = fmodf(curtime * 7200.f, 360.f);

		cmd->viewangles.pitch = 89.f;

		break;
	}

	default:
		break;
	}

	cmd->viewangles.Clamp();

	if (send_packet)
		fake = cmd->viewangles;
	else
		real = cmd->viewangles;
}

void antiaim::draw_angles()
{
	if (!options::antiaim::enabled || !options::antiaim::text || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	static auto font = draw::create_font("Verdana", 15);

	std::string lby_str = "lby: ";
	lby_str += std::to_string(localplayer->GetLBY());
	draw::text(100, 100, lby_str.c_str(), font, Color{ 255, 255, 255 });

	std::string fake_str = "fake yaw: ";
	fake_str += std::to_string(fake.yaw);
	draw::text(100, 125, fake_str.c_str(), font, Color{ 255, 255, 255 });

	std::string real_str = "real yaw: ";
	real_str += std::to_string(real.yaw);
	draw::text(100, 150, real_str.c_str(), font, Color{ 255, 255, 255 });

	std::string choking_str = "choking: ";
	choking_str += choking ? "true, " : "false, ";
	choking_str += std::to_string(choked_ticks);
	draw::text(100, 200, choking_str.c_str(), font, Color{ 255, 255, 255 });
}
