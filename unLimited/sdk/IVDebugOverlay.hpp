#pragma once

#include "../utils.hpp"

class Vector;

class IVDebugOverlay
{
public:
	bool ScreenPosition(const Vector &point, Vector &screen)
	{
		using oScreenPosition = bool(__thiscall*)(void*, const Vector&, Vector&);
		return get_vfunc<oScreenPosition>(this, 13)(this, point, screen);
	}

	bool ScreenPosition(float flXPos, float flYPos, Vector& screen)
	{
		using oScreenPosition = bool(__thiscall*)(void*, float, float, Vector&);
		return get_vfunc<oScreenPosition>(this, 14)(this, flXPos, flYPos, screen);
	}
};
