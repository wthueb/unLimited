#include "hooks.hpp"

#include "events.hpp"
#include "../utils.hpp"

uintptr_t d3d_device{};

namespace hooks
{
	std::shared_ptr<vmt_hook> d3d_device_hook{};
	std::shared_ptr<vmt_hook> client_hook{};
	std::shared_ptr<vmt_hook> client_mode_hook{};
	std::shared_ptr<vmt_hook> game_event_manager_hook{};
	std::shared_ptr<vmt_hook> panel_hook{};
	std::shared_ptr<vmt_hook> render_view_hook{};

	void init()
	{
		d3d_device = **reinterpret_cast<uintptr_t**>(utils::find_signature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

		d3d_device_hook = std::make_shared<vmt_hook>(d3d_device);
		client_hook = std::make_shared<vmt_hook>(g_client);
		client_mode_hook = std::make_shared<vmt_hook>(g_client_mode);
		game_event_manager_hook = std::make_shared<vmt_hook>(g_game_event_manager);
		panel_hook = std::make_shared<vmt_hook>(g_panel);
		render_view_hook = std::make_shared<vmt_hook>(g_render_view);

		d3d_device_hook->hook(hk_end_scene, index::end_scene);
		d3d_device_hook->hook(hk_reset, index::reset);

		client_hook->hook(hk_frame_stage_notify, index::frame_stage_notify);

		client_mode_hook->hook(hk_override_view, index::override_view);
		client_mode_hook->hook(hk_create_move, index::create_move);
		client_mode_hook->hook(hk_do_post_screen_space_effects, index::do_post_screen_space_effects);

		game_event_manager_hook->hook(events::hk_fire_event_client_side, index::fire_event_client_side);

		panel_hook->hook(hk_paint_traverse, index::paint_traverse);

		render_view_hook->hook(hk_scene_end, index::scene_end);

		events::init();
	}

	void unload()
	{
		d3d_device_hook->shutdown();
		client_hook->shutdown();
		client_mode_hook->shutdown();
		game_event_manager_hook->shutdown();
		panel_hook->shutdown();
		render_view_hook->shutdown();
	}
}
