#pragma once

#include "sdk/sdk.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#define RAD2DEG(x) (static_cast<float>(x) * static_cast<float>(180.f / M_PI))
#define DEG2RAD(x) (static_cast<float>(x) * static_cast<float>(M_PI / 180.f))

namespace math
{
	static void inline SinCos(float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	static void AngleVectors(const QAngle &angles, Vector* forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[1]), &sy, &cy);

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	static void VectorAngles(const Vector &forward, QAngle &angles)
	{
		if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // pitch
			angles[1] = 0.0f; // yaw
		}
		else
		{
			// pitch
			angles[0] = static_cast<vec_t>(atan2(-forward[2], forward.Length2D()) * -180 / M_PI);
			// yaw
			angles[1] = static_cast<vec_t>(atan2(forward[1], forward[0]) * 180 / M_PI);

			if (angles[1] > 90)
				angles[1] -= 180;
			else if (angles[1] < 90)
				angles[1] += 180;
			else if (angles[1] == 90)
				angles[1] = 0;
		}

		angles[2] = 0.0f;
	}

	static float distance_point_to_line(const Vector &point, const Vector &line_origin, const Vector &dir)
	{
		auto point_dir = point - line_origin;

		auto temp = point_dir.Dot(dir) / dir.LengthSqr();
		if (temp < 0.000001f)
			return FLT_MAX;

		auto perpendicular = line_origin + dir * temp;

		return (point - perpendicular).Length();
	}

	static float get_fov(const QAngle &view_angle, const QAngle &aim_angle)
	{
		Vector aim, ang;

		AngleVectors(view_angle, &aim);
		AngleVectors(aim_angle, &ang);

		return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
	}

	static void correct_movement(CUserCmd* cmd, QAngle old_angle, float old_forward, float old_side)
	{
		// side/forward move correction
		float delta;
		float f1;
		float f2;

		if (old_angle.yaw < 0.f)
			f1 = 360.0f + old_angle.yaw;
		else
			f1 = old_angle.yaw;

		if (cmd->viewangles.yaw < 0.0f)
			f2 = 360.0f + cmd->viewangles.yaw;
		else
			f2 = cmd->viewangles.yaw;

		if (f2 < f1)
			delta = abs(f2 - f1);
		else
			delta = 360.0f - abs(f1 - f2);

		delta = 360.0f - delta;

		cmd->forwardmove = cos(DEG2RAD(delta)) * old_forward + cos(DEG2RAD(delta + 90.f)) * old_side;
		cmd->sidemove = sin(DEG2RAD(delta)) * old_forward + sin(DEG2RAD(delta + 90.f)) * old_side;
	}

	static void clamp_angle(QAngle &angle)
	{
		for (auto i = 0; i < 3; ++i)
			if (!std::isfinite(angle[i])) // if it is infinite or NaN
				angle[i] = 0.f;

		angle.pitch = std::clamp(std::remainderf(angle.pitch, 180.f), -89.f, 89.f);
		angle.yaw = std::clamp(std::remainderf(angle.yaw, 360.f), -180.f, 180.f);
		angle.roll = 0.f;
	}
}