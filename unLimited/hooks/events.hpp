#pragma once

#include "../sdk/sdk.hpp"

namespace events
{
    bool __stdcall hk_fire_event_client_side(IGameEvent* event);
}
