#include "hooks.hpp"

#include "../features/features.hpp"
#include "../options.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

void __stdcall hooks::hk_override_view(CViewSetup* view)
{
	static auto o_override_view = client_mode_hook->get_original<void(__thiscall*)(void*, CViewSetup*)>(index::override_view);

	if (!g_engine->IsInGame())
		return o_override_view(g_client_mode, view);

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return o_override_view(g_client_mode, view);

	if (localplayer->GetObserverTarget().IsValid() ? !localplayer->GetObserverTarget()->IsScoped() : !localplayer->IsScoped())
		view->fov = options.misc_fov;
	else if (options.misc_fov_scoped)
		view->fov = options.misc_fov;

	visuals::thirdperson_override_view();
	
	o_override_view(g_client_mode, view);
}
