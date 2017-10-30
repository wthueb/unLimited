#include "hooks.hpp"

#include "../features/esp.hpp"

bool __stdcall hooks::hk_do_post_screen_space_effects(CViewSetup* view)
{
	static auto o_do_post_screen_space_effects = client_mode_hook->get_original<bool(__thiscall*)(void*, CViewSetup*)>(index::do_post_screen_space_effects);

	// FIXMEW: test of altering view will change fov, allowing you to remove OverrideView hook
	esp::do_post_screen_space_effects();

	return o_do_post_screen_space_effects(g_client_mode, view);
}
