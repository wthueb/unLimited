#include "hooks.hpp"

#include "../features/features.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

void __fastcall hooks::hk_paint_traverse(void* thisptr, void* edx, VPANEL panel, bool force_repaint, bool allow_force)
{
	static auto o_paint_traverse = panel_hook->get_original<void(__thiscall*)(void*, VPANEL, bool, bool)>(index::paint_traverse);

	o_paint_traverse(thisptr, panel, force_repaint, allow_force);

	static VPANEL FocusOverlayPanel = 0u;
	if (!FocusOverlayPanel)
	{
		auto panel_name = g_panel->GetName(panel);
		if (!strcmp(panel_name, "FocusOverlayPanel"))
			FocusOverlayPanel = panel;
		else
			return;
	}
	else if (panel != FocusOverlayPanel)
		return;

	backtracking::draw();
	antiaim::draw_angles();
}
