#include "hooks.hpp"

#include "../sdk/sdk.hpp"
#include "../utils.hpp"

namespace hooks
{
	std::unique_ptr<vmt_hook> d3d_device_hook = nullptr;
	std::unique_ptr<vmt_hook> engine_client_hook = nullptr;

	void init()
	{
		auto d3d_device = **reinterpret_cast<uintptr_t**>(utils::find_signature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

		d3d_device_hook = std::make_unique<vmt_hook>(d3d_device);
		engine_client_hook = std::make_unique<vmt_hook>(g_engine_client);

		d3d_device_hook->hook(reinterpret_cast<void*>(hk_end_scene), index::end_scene);
		d3d_device_hook->hook(reinterpret_cast<void*>(hk_reset), index::reset);
	}

	void unload()
	{
		d3d_device_hook->restore();
		engine_client_hook->restore();
	}
}
