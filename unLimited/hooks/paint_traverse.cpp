#include "hooks.hpp"

#include "../features/features.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

void __fastcall hooks::hk_paint_traverse(void* thisptr, void* edx, VPANEL panel, bool force_repaint, bool allow_force)
{
	static auto o_paint_traverse = panel_hook->get_original<void(__thiscall*)(void*, VPANEL, bool, bool)>(index::paint_traverse);

	static VPANEL FocusOverlayPanel = 0u;
	static VPANEL HudZoom = 0u;

	// don't want to have to get panel name and strcmp every single time
	if (!FocusOverlayPanel || !HudZoom)
	{
		auto panel_name = g_panel->GetName(panel);
		if (!FocusOverlayPanel && !strcmp(panel_name, "FocusOverlayPanel"))
			FocusOverlayPanel = panel;
		else if (!HudZoom && !strcmp(panel_name, "HudZoom"))
			HudZoom = panel;
	}

	if (panel != FocusOverlayPanel)
	{
		if (panel == HudZoom)
			return;
		else
			return o_paint_traverse(thisptr, panel, force_repaint, allow_force);
	}

	backtracking::draw();
	antiaim::draw_angles();

	o_paint_traverse(thisptr, panel, force_repaint, allow_force);
}
