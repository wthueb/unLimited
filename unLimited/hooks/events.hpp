#pragma once

#include <functional>
#include <string>

#include "../sdk/sdk.hpp"

namespace events
{
	void init();
	void unload();

	bool add_listener(std::string name, std::function<void(IGameEvent*)>);

	bool __stdcall hk_fire_event_client_side(IGameEvent* event);
}
