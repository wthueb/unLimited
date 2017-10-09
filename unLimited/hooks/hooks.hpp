#pragma once

#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

#include <memory>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace hooks
{
	void init();
	void unload();

	HRESULT __stdcall hk_end_scene(IDirect3DDevice9* pDevice);
	HRESULT __stdcall hk_reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	extern std::unique_ptr<vmt_hook> d3d_device_hook;
	extern std::unique_ptr<vmt_hook> engine_client_hook;

	namespace index
	{
		constexpr size_t reset = 16;
		constexpr size_t end_scene = 42;
	}
}
