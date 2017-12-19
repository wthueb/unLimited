#include "features.hpp"

#include <fstream>
#include <sstream>

#include "../engine_drawing.hpp"
#include "../options.hpp"
#include "../sdk/sdk.hpp"

IMaterial* create_materal(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert)
{
	std::stringstream mat_data;
	mat_data << "\"" + type + "\"\n"
		"{\n"
		"\t\"$basetexture\" \"" + texture + "\"\n"
		"\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
		"\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
		"\t\"$model\" \"" + std::to_string(model) + "\"\n"
		"\t\"$nocull\" \"" + std::to_string(nocull) + "\"\n"
		"\t\"$halflambert\" \"" + std::to_string(halflambert) + "\"\n"
		"}\n" << std::flush;

	static auto num = 0;
	std::string mat_name = "unLimited_" + std::to_string(num++);

	auto key_values = new KeyValues(mat_name.c_str());
	key_values->LoadFromBuffer(mat_name.c_str(), mat_data.str().c_str());

	return g_material_system->CreateMaterial(mat_name.c_str(), key_values);
}


void override_material(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& col = Color{ 255, 255, 255 })
{
	static IMaterial* mat_regular = create_materal("VertexLitGeneric", "vgui/white_additive", false, true, true, true, true);
	static IMaterial* mat_regular_ignorez = create_materal("VertexLitGeneric", "vgui/white_additive", true, true, true, true, true);

	static IMaterial* mat_flat = create_materal("UnlitGeneric", "vgui/white_additive", false, true, true, true, true);
	static IMaterial* mat_flat_ignorez = create_materal("UnlitGeneric", "vgui/white_additive", true, true, true, true, true);

	IMaterial* material = nullptr;

	if (flat)
	{
		if (ignoreZ)
			material = mat_flat_ignorez;
		else
			material = mat_flat;
	}
	else
	{
		if (ignoreZ)
			material = mat_regular_ignorez;
		else
			material = mat_regular;
	}


	if (glass)
	{
		material = mat_flat;
		material->AlphaModulate(0.45f);
	}
	else
	{
		material->AlphaModulate(col.a() / 255.f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);

	material->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);

	g_model_render->ForcedMaterialOverride(material);
}


void visuals::chams()
{
	if (!options.visuals_enabled || !options.visuals_chams || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!player || !player->IsValid())
			continue;
		
		if (!g_input->m_fCameraInThirdPerson && player == localplayer)
			continue;

		if (!options.visuals_friendlies && player->GetTeam() == localplayer->GetTeam())
			continue;

		// "dev/dev_envmap"
		// "models/inventory_items/trophy_majors/crystal_blue"
		// "models/inventory_items/dogtags/dogtags_outline"

		if (options.visuals_chams_ignorez)
		{
			g_render_view->SetColorModulation(.6f, .3f, .3f); // faded red
			g_render_view->SetBlend(1.f);

			override_material(true, options.visuals_chams_flat, options.visuals_chams_wireframe, false);

			player->DrawModel(STUDIO_RENDER, 255);
		}

		g_render_view->SetColorModulation(.3f, .5f, .7f); // faded blue
		g_render_view->SetBlend(options.visuals_chams_glass && !options.visuals_chams_ignorez ? .45f : 1.f);

		override_material(false, options.visuals_chams_flat, options.visuals_chams_wireframe, options.visuals_chams_ignorez ? false : options.visuals_chams_glass);

		player->DrawModel(STUDIO_RENDER, 255);

		g_model_render->ForcedMaterialOverride(nullptr);
	}
}

void visuals::chams2(DrawModelExecuteFn o_draw_model_execute, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix)
{
	if (!options.visuals_enabled || !options.visuals_chams || !g_engine->IsInGame())
		return;

	const auto mdl = info.pModel;

	auto is_arm = !!strstr(mdl->szName, "arms");
	auto is_player = !!strstr(mdl->szName, "models/player");
	auto is_sleeve = !!strstr(mdl->szName, "sleeve");
	auto is_weapon = !!strstr(mdl->szName, "weapons/v_");

	/*if (is_player && options.visuals_chams_players)
	{
		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(info.entity_index));
		if (!player || !player->IsValid())
			return;

		auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
		if (!localplayer)
			return;

		auto is_enemy = player->GetTeam() != localplayer->GetTeam();

		if (!options.visuals_friendlies && !is_enemy)
			return;

		const auto clr_front = is_enemy ? Color{ 100, 255, 100 } : Color{ 100, 100, 255 };
		const auto clr_back = is_enemy ? Color{ 255, 100, 100 } : Color{ 255, 120, 80 };

		if (options.visuals_chams_ignorez)
		{
			override_material(true, options.visuals_chams_flat, options.visuals_chams_wireframe, false, clr_back);
			
			o_draw_model_execute(g_model_render, ctx, state, info, matrix);

			override_material(false, options.visuals_chams_flat, options.visuals_chams_wireframe, false, clr_front);
		}
		else
		{
			override_material(false, options.visuals_chams_flat, options.visuals_chams_wireframe, options.visuals_chams_glass, clr_front);
		}
	}
	else */if (is_sleeve && options.visuals_chams_arms)
	{
		auto material = g_material_system->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		if (!material)
			return;

		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

		g_model_render->ForcedMaterialOverride(material);
	}
	else if (is_arm)
	{
		auto material = g_material_system->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		if (!material)
			return;

		if (options.visuals_chams_no_hands)
		{
			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_model_render->ForcedMaterialOverride(material);
		}
		else if (options.visuals_chams_arms)
		{
			if (options.visuals_chams_ignorez)
			{
				override_material(true, options.visuals_chams_flat, options.visuals_chams_wireframe, false, Color{ .6f, .3f, .3f });

				o_draw_model_execute(g_model_render, ctx, state, info, matrix);

				override_material(false, options.visuals_chams_flat, options.visuals_chams_wireframe, false, Color{ .3f, .5f, .7f });
			}
			else
			{
				override_material(false, options.visuals_chams_flat, options.visuals_chams_wireframe, options.visuals_chams_glass, Color{ 255, 255, 255 });
			}
		}
	}
}

void visuals::glow()
{
	if (!options.visuals_enabled || !options.visuals_glow || !g_engine->IsInGame())
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
			if (!options.visuals_players)
				continue;

			auto player = static_cast<C_BasePlayer*>(entity);

			if (!player->IsAlive())
				continue;

			if (!options.visuals_friendlies && player->GetTeam() == localplayer->GetTeam())
				continue;

			// FIXMEW: add color picker
			color = player->GetTeam() == localplayer->GetTeam() ? Color{ 0, 255, 0 } : Color{ 255, 0, 0 };

			break;
		}
		case ClassId_CChicken:
		{
			if (!options.visuals_chickens)
				continue;

			entity->GetShouldGlow() = true;

			color = Color{ 255, 255, 255 };

			break;
		}
		case ClassId_CBaseAnimating:
		{
			if (!options.visuals_defuse_kits)
				continue;

			color = Color{ 255, 255, 255 };

			break;
		}
		case ClassId_CPlantedC4:
		{
			if (!options.visuals_c4)
				continue;

			color = Color{ 255, 255, 255 };

			break;
		}
		default:
		{
			if (entity->IsWeapon())
			{
				if (!options.visuals_weapons)
					continue;

				color = Color{ 255, 255, 255 };
			}

			break;
		}
		}

		object.m_vecGlowColor = Vector{ color.r() / 255.f, color.g() / 255.f, color.b() / 255.f };
		object.m_flGlowAlpha = options.visuals_glow_alpha;
		object.m_flBloomAmount = 1.f;
		object.m_bRenderWhenOccluded = true;
		object.m_bRenderWhenUnoccluded = false;
		object.m_nGlowStyle = options.visuals_glow_style;
	}
}

void visuals::skeletons()
{
	if (!options.visuals_enabled || !options.visuals_skeletons || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!player)
			continue;

		if (!player->IsValid() || !player->IsPlayer() || player == localplayer)
			continue;

		if (player == localplayer->GetObserverTarget())
			continue;

		studiohdr_t* studio_model = g_model_info->GetStudioModel(player->GetModel());
		if (!studio_model)
			continue;
		
		static matrix3x4_t bone_to_world_out[128];

		if (player->SetupBones(bone_to_world_out, 128, 256, player->GetSimulationTime()))
		{
			for (auto i = 0; i < studio_model->numbones; ++i)
			{
				mstudiobone_t* bone = studio_model->pBone(i);
				if (!bone || !(bone->flags & 256) || bone->parent == -1)
					continue;

				Vector bone_pos;
				if (g_debug_overlay->ScreenPosition(player->GetBonePos(Bone(i)), bone_pos))
					continue;

				Vector parent_pos;
				if (g_debug_overlay->ScreenPosition(player->GetBonePos(Bone(bone->parent)), parent_pos))
					continue;

				draw::line(Vector2D{ bone_pos.x, bone_pos.y }, Vector2D{ parent_pos.x, parent_pos.y }, Color{ 255, 255, 255 });
			}
		}
	}
}

void visuals::radar()
{
	if (!options.visuals_enabled || !options.visuals_radar || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));

	for (auto i = 0; i < g_engine->GetMaxClients(); ++i)
	{
		auto player = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(i));
		if (!player)
			continue;

		if (!player->IsValid() || !player->IsPlayer() || player == localplayer)
			continue;

		player->GetSpotted() = true;
	}
}

void visuals::thirdperson_override_view()
{
	if (!g_engine->IsInGame())
		return;

	if (!options.visuals_enabled || !options.visuals_thirdperson)
	{
		g_input->m_fCameraInThirdPerson = false;
		return;
	}
	
	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	static QAngle viewangles;
	g_engine->GetViewAngles(viewangles);

	if (localplayer->IsAlive() && !localplayer->IsScoped())
	{
		g_input->m_fCameraInThirdPerson = true;
		g_input->m_vecCameraOffset = Vector{ viewangles.pitch, viewangles.yaw, options.visuals_thirdperson_offset };
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
	if (!options.visuals_enabled || !options.visuals_thirdperson || !g_engine->IsInGame())
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

void visuals::noflash()
{
	if (!options.visuals_enabled || !options.visuals_noflash || !g_engine->IsInGame())
		return;

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer)
		return;

	localplayer->GetFlashMaxAlpha() = 0.f;
}
