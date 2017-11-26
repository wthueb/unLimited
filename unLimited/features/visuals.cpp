#include "features.hpp"

#include <fstream>

#include "../options.hpp"
#include "../sdk/sdk.hpp"

void visuals::chams()
{
	if (!options::visuals::enabled || !options::visuals::chams || !g_engine->IsInGame())
		return;

	static bool initialized = false;

	if (!initialized)
	{
		std::ofstream("csgo\\materials\\regular.vmt") << R"#("VertexLitGeneric"
			{
			"$basetexture" "vgui/white_additive"
			"$ignorez"      "0"
			"$envmap"       ""
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "1"
			}
		)#";

		std::ofstream("csgo\\materials\\ignorez.vmt") << R"#("VertexLitGeneric"
			{
			"$basetexture" "vgui/white_additive"
			"$ignorez"      "1"
			"$envmap"       ""
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "1"
			}
		)#";

		initialized = true;
	}

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		if (i == g_engine->GetLocalPlayer())
			continue;

		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!player)
			continue;

		if (!player->IsValid())
			continue;

		//IMaterial* mat = g_material_system->FindMaterial("dev/dev_envmap", TEXTURE_GROUP_OTHER);
		//IMaterial* mat = g_material_system->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER);
		//IMaterial* mat = g_material_system->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_OTHER);

		if (options::visuals::ignorez)
		{
			static IMaterial* ignorez = g_material_system->FindMaterial("ignorez", TEXTURE_GROUP_MODEL);
			if (!ignorez)
				continue;

			ignorez->ColorModulate(1.f, 0.f, 0.f);
			ignorez->AlphaModulate(1.f);

			g_model_render->ForcedMaterialOverride(ignorez);
			player->DrawModel(STUDIO_RENDER, 255);
		}

		static IMaterial* regular = g_material_system->FindMaterial("regular", TEXTURE_GROUP_MODEL);
		if (!regular)
			continue;

		regular->ColorModulate(0.f, 1.f, 0.f);
		regular->AlphaModulate(1.f);

		g_model_render->ForcedMaterialOverride(regular);
		player->DrawModel(STUDIO_RENDER, 255);

		g_model_render->ForcedMaterialOverride(nullptr);
	}
}

void visuals::glow()
{
	if (!options::visuals::enabled || !options::visuals::glow || !g_engine->IsInGame())
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

void visuals::radar()
{
	if (!options::visuals::enabled || !options::visuals::radar || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!player)
			continue;

		if (!player->IsValid() || player == localplayer)
			continue;

		player->GetSpotted() = true;
	}
}

void visuals::thirdperson_override_view()
{
	if (!options::visuals::enabled || !options::visuals::thirdperson || !g_engine->IsInGame())
	{
		g_input->m_fCameraInThirdPerson = false;
		return;
	}
	
	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	static QAngle viewangles;
	g_engine->GetViewAngles(viewangles);

	if (localplayer->IsAlive())
	{
		g_input->m_fCameraInThirdPerson = true;
		g_input->m_vecCameraOffset = Vector{ viewangles.pitch, viewangles.yaw, vec_t(options::visuals::thirdperson_offset) };
	}
	else
	{
		g_input->m_fCameraInThirdPerson = false;
	}
}

// defined in antiaim.cpp
extern QAngle g_thirdperson_angles;

void visuals::thirdperson_fsn()
{
	if (!options::visuals::enabled || !options::visuals::thirdperson || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	static ptrdiff_t deadflag = netvar_sys::get().get_offset("DT_BasePlayer", "deadflag");

	if (localplayer->IsAlive() && g_input->m_fCameraInThirdPerson)
	{
		*reinterpret_cast<QAngle*>(uintptr_t(localplayer) + deadflag + 4) = g_thirdperson_angles;
	}
}
