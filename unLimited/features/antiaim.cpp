#include "features.hpp"

#include "../engine_drawing.hpp"
#include "../math.hpp"
#include "../options.hpp"

#define TICKS_TO_TIME(t)  (float(t) * g_global_vars->interval_per_tick)
#define TIME_TO_TICKS(dt) (int(.5f + float(dt) / g_global_vars->interval_per_tick))

QAngle g_real{};
QAngle g_fake{};

// declared in misc.cpp
extern bool g_choking;
extern int g_choked_ticks;

QAngle g_thirdperson_angles{};

#define RETURN { g_thirdperson_angles = g_real = g_fake = cmd->viewangles; return; }

void antiaim::process(CUserCmd* cmd, bool& send_packet)
{
	if (!g_engine->IsInGame())
		return;

	if (!options.aa_enabled)
	{
		g_thirdperson_angles = cmd->viewangles;
		return;
	}

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		RETURN;

	if (!localplayer->IsAlive())
		RETURN;

	auto weapon = localplayer->GetActiveWeapon().Get();
	if (!weapon || weapon->IsKnife() || weapon->IsBomb())
		RETURN;

	if (cmd->buttons & IN_USE)
		RETURN;

	if (weapon->IsNade())
	{
		auto nade = static_cast<C_BaseCSGrenade*>(weapon);

		if (nade->GetThrowTime() > 0.f)
			RETURN;
	}

	if (cmd->buttons & IN_ATTACK &&
		weapon->GetNextPrimaryAttack() <= g_global_vars->get_realtime(cmd) &&
		weapon->GetAmmo() > 0)
		RETURN;

	/*if (localplayer->GetMoveType() == MOVETYPE_LADDER)
		return;*/

	switch (options.aa_type)
	{
	case aa_type::LEGIT:
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

	case aa_type::RAGE: // fake sideways, emotion; real backwards, emotion
	{
		if (send_packet)
			cmd->viewangles.yaw += 90.f;
		else
			cmd->viewangles.yaw += 180.f;

		cmd->viewangles.pitch = 89.f;

		break;
	}

	case aa_type::LBY_SIDEWAYS: // fake backwards, emotion; real sideways opposite lby, emotion
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

	case aa_type::SPIN_SLOW:
	{
		float curtime = g_global_vars->get_realtime(cmd);

		cmd->viewangles.yaw = fmodf(curtime * 300.f, 360.f);

		cmd->viewangles.pitch = 89.f;

		break;
	}

	case aa_type::SPIN_FAST:
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
		g_fake = cmd->viewangles;
	else
		g_real = cmd->viewangles;

	if (send_packet)
	{
		// if not faking, display real angle
		if (options.aa_type == aa_type::SPIN_SLOW || options.aa_type == aa_type::SPIN_FAST)
			g_thirdperson_angles = cmd->viewangles;
	}
	else
	{
		// if faking, display fake angle in thirdperson
		if (options.aa_type == aa_type::LEGIT || options.aa_type == aa_type::RAGE ||
			options.aa_type == aa_type::LBY_SIDEWAYS)
			g_thirdperson_angles = cmd->viewangles;
	}
}

void antiaim::draw_angles()
{
	if (!options.aa_enabled || !options.aa_show || !g_engine->IsInGame())
		return;

	static auto font = draw::create_font("Verdana", 15);

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;
	
	if (!localplayer->IsAlive())
		return;

	std::string lby_str{ "lby: " + std::to_string(localplayer->GetLBY()) };
	draw::text(100, 100, lby_str.c_str(), font, Color{ 0, 0, 255 });

	std::string real_str{ "real yaw: " + std::to_string(g_real.yaw) };
	draw::text(100, 150, real_str.c_str(), font, Color{ 0, 255, 0 });

	std::string fake_str{ "fake yaw: " + std::to_string(g_fake.yaw) };
	draw::text(100, 125, fake_str.c_str(), font, Color{ 255, 0, 0 });

	std::string choking_str{ std::string{ "choking: " } + (g_choking ? "true, " : "false, ") + std::to_string(g_choked_ticks) };
	draw::text(100, 200, choking_str.c_str(), font, Color{ 255, 255, 255 });

	constexpr auto line_length = 20.f;

	auto start = localplayer->GetOrigin();

	Vector forward, end, start2d, end2d;
	
	// lby
	{
		math::angle_vectors(QAngle{ 0.f, localplayer->GetLBY(), 0.f }, &forward);
		
		forward *= line_length * 1.5f;
		
		end = start + forward;
		
		if (g_debug_overlay->ScreenPosition(start, start2d) || g_debug_overlay->ScreenPosition(end, end2d))
			return;
		
		draw::line(int(start2d.x), int(start2d.y), int(end2d.x), int(end2d.y), Color{ 0, 0, 255 });
		draw::text(int(end2d.x), int(end2d.y), "lby", font, Color{ 0, 0, 255 });
	}

	// real
	{
		math::angle_vectors(QAngle{ 0.f, g_real.yaw, 0.f }, &forward);

		forward *= line_length;

		end = start + forward;

		if (g_debug_overlay->ScreenPosition(start, start2d) || g_debug_overlay->ScreenPosition(end, end2d))
			return;

		draw::line(int(start2d.x), int(start2d.y), int(end2d.x), int(end2d.y), Color{ 0, 255, 0 });
		draw::text(int(end2d.x), int(end2d.y), "real", font, Color{ 0, 255, 0 });
	}

	// fake
	{
		math::angle_vectors(QAngle{ 0.f, g_fake.yaw, 0.f }, &forward);

		forward *= line_length;

		end = start + forward;

		if (g_debug_overlay->ScreenPosition(start, start2d) || g_debug_overlay->ScreenPosition(end, end2d))
			return;

		draw::line(int(start2d.x), int(start2d.y), int(end2d.x), int(end2d.y), Color{ 255, 0, 0 });
		draw::text(int(end2d.x), int(end2d.y), "fake", font, Color{ 255, 0, 0 });
	}
}
