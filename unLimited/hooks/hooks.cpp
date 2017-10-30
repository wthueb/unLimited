#include "hooks.hpp"

#include "../sdk/sdk.hpp"
#include "../utils.hpp"

uintptr_t d3d_device{};

namespace hooks
{
	vmt_hook* d3d_device_hook = nullptr;
	vmt_hook* client_mode_hook = nullptr;
	vmt_hook* panel_hook = nullptr;

	void init()
	{
		d3d_device = **reinterpret_cast<uintptr_t**>(utils::find_signature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

		d3d_device_hook = new vmt_hook(d3d_device);
		client_mode_hook = new vmt_hook(g_client_mode);
		panel_hook = new vmt_hook(g_panel);

		d3d_device_hook->hook(hk_end_scene, index::end_scene);
		d3d_device_hook->hook(hk_reset, index::reset);

		client_mode_hook->hook(hk_override_view, index::override_view);
		client_mode_hook->hook(hk_create_move, index::create_move);
		client_mode_hook->hook(hk_do_post_screen_space_effects, index::do_post_screen_space_effects);

		panel_hook->hook(hk_paint_traverse, index::paint_traverse);
	}

	void unload()
	{
		d3d_device_hook->shutdown();
		client_mode_hook->shutdown();
		panel_hook->shutdown();
	}
}
