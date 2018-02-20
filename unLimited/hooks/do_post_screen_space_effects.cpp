#include "hooks.hpp"

#include "../features/features.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

bool __stdcall hooks::hk_do_post_screen_space_effects(CViewSetup* view)
{
	static const auto o_do_post_screen_space_effects = client_mode_hook->get_original<bool(__thiscall*)(void*, CViewSetup*)>(index::do_post_screen_space_effects);

	// FIXMEW: test if altering the view parameter will change fov, allowing to remove the OverrideView hook
	visuals::glow();

	return o_do_post_screen_space_effects(g_client_mode, view);
}
