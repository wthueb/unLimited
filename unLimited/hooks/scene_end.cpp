#include "hooks.hpp"

#include "../features/features.hpp"
#include "../vmt_hook.hpp"

void __fastcall hooks::hk_scene_end(void* thisptr, void* edx)
{
	static auto o_scene_end = render_view_hook->get_original<void(__thiscall*)(void*)>(index::scene_end);

	o_scene_end(thisptr);

	//visuals::chams();
}
