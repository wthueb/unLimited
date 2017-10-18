#pragma once

#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace hooks
{
	void init();
	void unload();

	HRESULT __stdcall hk_reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	HRESULT __stdcall hk_end_scene(IDirect3DDevice9* pDevice);

	bool __stdcall hk_create_move(float sample_input_frametime, CUserCmd* cmd);

	extern vmt_hook* d3d_device_hook;
	extern vmt_hook* client_mode_hook;

	namespace index
	{
		constexpr size_t reset = 16;
		constexpr size_t end_scene = 42;

		constexpr size_t create_move = 24;
	}
}
