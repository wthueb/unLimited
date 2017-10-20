#pragma once

#include "sdk/sdk.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

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
}
