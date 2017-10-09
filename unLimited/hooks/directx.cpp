#include "hooks.hpp"

HRESULT __stdcall hooks::hk_end_scene(IDirect3DDevice9* pDevice)
{
	static auto o_end_scene = d3d_device_hook->get_original<HRESULT(__stdcall*)(IDirect3DDevice9*)>(index::end_scene);

	return o_end_scene(pDevice);
}

HRESULT __stdcall hooks::hk_reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	static auto o_reset = d3d_device_hook->get_original<HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)>(index::reset);

	auto ret = o_reset(pDevice, pPresentationParameters);

	return ret;
}
