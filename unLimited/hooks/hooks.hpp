#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <memory>

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
	void __stdcall hk_frame_stage_notify(ClientFrameStage stage);
	bool __fastcall hk_dispatch_user_message(void* thisptr, void* edx, int type, unsigned int a3, unsigned int length, const void* msg_data);
	
	// client mode
	void __stdcall hk_override_view(CViewSetup* view);
	bool __stdcall hk_create_move(float sample_input_frametime, CUserCmd* cmd);
	bool __stdcall hk_do_post_screen_space_effects(CViewSetup* view);

	// engine sound
	void __fastcall hk_emit_sound(void* thisptr, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const Vector* pOrigin = nullptr, const Vector* pDirection = nullptr, CUtlVector<Vector>* pUtlVecOrigins = nullptr, bool bUpdatePositions = true, float soundtime = 0.f, int speakerentity = -1);

	// game event manager in events.hpp

	// model render
	void __stdcall hk_draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix);

	// panel
	void __fastcall hk_paint_traverse(void* thisptr, void* edx, VPANEL panel, bool force_repaint, bool allow_force);

	// render view
	void __fastcall hk_scene_end(void* thisptr, void* edx);

	extern std::shared_ptr<vmt_hook> d3d_device_hook;
	extern std::shared_ptr<vmt_hook> client_hook;
	extern std::shared_ptr<vmt_hook> client_mode_hook;
	extern std::shared_ptr<vmt_hook> engine_sound_hook;
	extern std::shared_ptr<vmt_hook> game_event_manager_hook;
	extern std::shared_ptr<vmt_hook> model_render_hook;
	extern std::shared_ptr<vmt_hook> panel_hook;
	extern std::shared_ptr<vmt_hook> render_view_hook;

	namespace index
	{
		// d3d
		static constexpr auto reset = 16u;
		static constexpr auto end_scene = 42u;

		// client
		static constexpr auto frame_stage_notify = 37u;
		static constexpr auto dispatch_user_message = 38u;

		// client mode
		static constexpr auto override_view = 18u;
		static constexpr auto create_move = 24u;
		static constexpr auto do_post_screen_space_effects = 44u;

		// engine sound
		static constexpr auto emit_sound = 5u;

		// game event manager
		static constexpr auto fire_event_client_side = 9u;

		// model render
		static constexpr auto draw_model_execute = 21u;

		// panel
		static constexpr auto paint_traverse = 41u;

		// render view
		static constexpr auto scene_end = 9u;
	}
}
