#include "hooks.hpp"

#include "../sdk/sdk.hpp"
#include "../utils.hpp"

uintptr_t d3d_device{};

namespace hooks
{
	vmt_hook* d3d_device_hook = nullptr;
	vmt_hook* engine_client_hook = nullptr;

	void init()
	{
		d3d_device = **reinterpret_cast<uintptr_t**>(utils::find_signature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

		d3d_device_hook = new vmt_hook(d3d_device);
		engine_client_hook = new vmt_hook(g_engine);

		d3d_device_hook->hook(hk_end_scene, index::end_scene);
		d3d_device_hook->hook(hk_reset, index::reset);
	}

	void unload()
	{
		delete d3d_device_hook;
		delete engine_client_hook;
	}
}
