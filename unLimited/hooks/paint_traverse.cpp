#include "hooks.hpp"

#include "../engine_drawing.hpp"
#include "../features/features.hpp"
#include "../options.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

void __fastcall hooks::hk_paint_traverse(void* thisptr, void* edx, VPANEL panel, bool force_repaint, bool allow_force)
{
	static const auto o_paint_traverse = panel_hook->get_original<void(__thiscall*)(void*, VPANEL, bool, bool)>(index::paint_traverse);

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
		if (options.visuals_noscope && panel == HudZoom)
			return;
		else
			return o_paint_traverse(thisptr, panel, force_repaint, allow_force);
	}

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	
	if (options.visuals_noscope && localplayer && localplayer->IsScoped())
	{
		int w, h;
		g_engine->GetScreenSize(w, h);
		draw::line(0, h / 2, w, h / 2, Color{ 0, 0, 0 });
		draw::line(w / 2, 0, w / 2, h, Color{ 0, 0, 0 });
	}

	backtracking::draw();
	antiaim::draw_angles();
	visuals::skeletons();

	o_paint_traverse(thisptr, panel, force_repaint, allow_force);
}
