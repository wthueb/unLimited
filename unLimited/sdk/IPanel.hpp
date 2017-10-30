#pragma once

#include "../utils.hpp"

class IPanel
{
public:
	const char* GetName(unsigned int vguiPanel)
	{
		using oGetName = const char*(__thiscall*)(void*, unsigned int);
		return get_vfunc<oGetName>(this, 36)(this, vguiPanel);
	}
#ifdef GetClassName
#undef GetClassName
#endif
	const char* GetClassName(unsigned int vguiPanel)
	{
		using oGetClassName = const char*(__thiscall*)(void*, unsigned int);
		return get_vfunc<oGetClassName>(this, 37)(this, vguiPanel);
	}
};

extern IPanel* g_panel;
