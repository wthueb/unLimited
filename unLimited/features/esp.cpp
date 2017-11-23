#include "features.hpp"

#include "../sdk/sdk.hpp"

#include "../options.hpp"

void esp::glow()
{
	if (!options::visuals::enabled || !options::visuals::glow)
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	for (auto i = 0; i < g_glow_manager->m_GlowObjectDefinitions.m_Size; ++i)
	{
		auto& object = g_glow_manager->m_GlowObjectDefinitions[i];
		if (object.IsUnused())
			continue;

		auto entity = static_cast<C_BaseEntity*>(object.m_pEntity);
		if (!entity || entity->IsDormant())
			continue;

		auto class_id = entity->GetClientClass()->m_ClassID;
		Color color;

		switch (class_id)
		{
		case ClassId_CCSPlayer:
		{
			if (!options::visuals::players)
				continue;

			auto player = static_cast<C_BasePlayer*>(entity);

			if (!player->IsAlive())
				continue;

			if (!options::visuals::friendlies && player->GetTeam() == localplayer->GetTeam())
				continue;

			// FIXMEW: add color picker
			color = player->GetTeam() == localplayer->GetTeam() ? Color{ 0, 255, 0 } : Color{ 255, 0, 0 };

			break;
		}
		case ClassId_CChicken:
		{
			if (!options::visuals::chickens)
				continue;

			entity->GetShouldGlow() = true;

			color = Color{ 255, 255, 255 };

			break;
		}
		case ClassId_CBaseAnimating:
		{
			if (!options::visuals::defuse_kits)
				continue;

			color = Color{ 255, 255, 255 };

			break;
		}
		case ClassId_CPlantedC4:
		{
			if (!options::visuals::c4)
				continue;

			color = Color{ 255, 255, 255 };

			break;
		}
		default:
		{
			if (entity->IsWeapon())
			{
				if (!options::visuals::weapons)
					continue;

				color = Color{ 255, 255, 255 };
			}

			break;
		}
		}

		object.m_vecGlowColor = Vector{ color.r() / 255.f, color.g() / 255.f, color.b() / 255.f };
		object.m_flGlowAlpha = options::visuals::glow_alpha;
		object.m_flBloomAmount = 1.f;
		object.m_bRenderWhenOccluded = true;
		object.m_bRenderWhenUnoccluded = false;
		object.m_nGlowStyle = options::visuals::glow_style;
	}
}

void esp::radar()
{
	if (!options::visuals::radar || !g_engine->IsInGame())
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
