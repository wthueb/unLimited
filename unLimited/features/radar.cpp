#include "misc.hpp"

#include "../options.hpp"

void misc::radar()
{
	if (!options::esp::radar || !g_engine->IsInGame())
		return;
	
	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto entity = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!entity)
			continue;

		if (entity == localplayer || entity->IsDormant() || !entity->IsAlive())
			continue;

		entity->GetSpotted() = true;
	}
}
