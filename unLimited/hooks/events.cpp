#include "events.hpp"

#include "../features/features.hpp"
#include "hooks.hpp"
#include "../notifier.hpp"
#include "../sdk/sdk.hpp"

// FIXMEW: use listeners instead

bool __stdcall events::hk_fire_event_client_side(IGameEvent* event)
{
    static const auto o_fire_event_client_side = hooks::game_event_manager_hook->get_original<bool(__thiscall*)(void*, IGameEvent*)>(hooks::index::fire_event_client_side);

    if (!strcmp(event->GetName(), "player_death"))
    {
        skinchanger::fix_icons(event);
    }
    else if (!strcmp(event->GetName(), "round_start"))
    {
        notifier::update_all_options();
    }

    return o_fire_event_client_side(g_game_event_manager, event);
}
