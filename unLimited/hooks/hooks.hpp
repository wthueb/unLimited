#pragma once

#include <memory>

#include <d3d9.h>
#include <d3dx9.h>

#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace hooks
{
	void init();
	void unload();

	// d3d
	HRESULT __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters);
	HRESULT __stdcall hk_end_scene(IDirect3DDevice9* device);

	// client
	void __stdcall hk_frame_stage_notify(ClientFrameStage_t stage);
	
	// client mode
	void __stdcall hk_override_view(CViewSetup* view);
	bool __stdcall hk_create_move(float sample_input_frametime, CUserCmd* cmd);
	bool __stdcall hk_do_post_screen_space_effects(CViewSetup* view);

	// game event manager in events.hpp

	// panel
	void __fastcall hk_paint_traverse(void* thisptr, void* edx, VPANEL panel, bool force_repaint, bool allow_force);

	// render view
	void __fastcall hk_scene_end(void* thisptr, void* edx);

	extern std::shared_ptr<vmt_hook> d3d_device_hook;
	extern std::shared_ptr<vmt_hook> client_hook;
	extern std::shared_ptr<vmt_hook> client_mode_hook;
	extern std::shared_ptr<vmt_hook> game_event_manager_hook;
	extern std::shared_ptr<vmt_hook> panel_hook;
	extern std::shared_ptr<vmt_hook> render_view_hook;

	namespace index
	{
		// d3d
		static constexpr size_t reset = 16u;
		static constexpr size_t end_scene = 42u;

		// client
		static constexpr size_t frame_stage_notify = 36u;

		// client mode
		static constexpr size_t override_view = 18u;
		static constexpr size_t create_move = 24u;
		static constexpr size_t do_post_screen_space_effects = 44u;

		// game event manager
		static constexpr size_t fire_event_client_side = 9u;

		// panel
		static constexpr size_t paint_traverse = 41u;

		// render view
		static constexpr size_t scene_end = 9u;
	}
}
