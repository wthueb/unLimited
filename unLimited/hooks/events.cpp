#include "events.hpp"

#include <functional>
#include <string>

#include "../features/features.hpp"
#include "hooks.hpp"
#include "../notifier.hpp"
#include "../sdk/sdk.hpp"

void events::init()
{
	add_listener("round_end", [](IGameEvent* event)
	{
		notifier::update_all_options();
	});
}

bool events::add_listener(std::string name, std::function<void(IGameEvent*)> func)
{
	return g_game_event_manager->AddListener(new EventListener{ name, func }, name.c_str(), false);
}

bool __stdcall events::hk_fire_event_client_side(IGameEvent* event)
{
	static auto o_fire_event_client_side = hooks::game_event_manager_hook->get_original<bool(__thiscall*)(void*, IGameEvent*)>(hooks::index::fire_event_client_side);

	if (!strcmp(event->GetName(), "player_death"))
	{
		skinchanger::fix_icons(event);
	}

	return o_fire_event_client_side(g_game_event_manager, event);
}
