#include "hooks.hpp"

#include <fstream>

#include "../vmt_hook.hpp"

void __fastcall hooks::hk_scene_end(void* thisptr, void* edx)
{
	static auto o_scene_end = render_view_hook->get_original<void(__thiscall*)(void*)>(index::scene_end);

	//o_scene_end(thisptr);

	/*static bool initialized = false;

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

		IMaterial* ignorez = g_material_system->FindMaterial("ignorez", TEXTURE_GROUP_MODEL);
		if (!ignorez)
			continue;

		ignorez->ColorModulate(1.f, 0.f, 0.f);
		ignorez->AlphaModulate(1.f);
		
		g_model_render->ForcedMaterialOverride(ignorez);
		player->DrawModel(STUDIO_RENDER, 255);

		IMaterial* regular = g_material_system->FindMaterial("regular", TEXTURE_GROUP_MODEL);
		if (!regular)
			continue;

		regular->ColorModulate(0.f, 1.f, 0.f);
		regular->AlphaModulate(1.f);

		g_model_render->ForcedMaterialOverride(regular);
		player->DrawModel(STUDIO_RENDER, 255);

		g_model_render->ForcedMaterialOverride(nullptr);
	}*/
}
