#include "hooks.hpp"

#include "../options.hpp"

void __stdcall hooks::hk_override_view(CViewSetup* view)
{
	static auto o_override_view = client_mode_hook->get_original<void(__thiscall*)(void*, CViewSetup*)>(index::override_view);

	view->fov = options::misc::fov;

	o_override_view(g_client_mode, view);
}
