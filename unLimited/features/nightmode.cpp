#include "misc.hpp"

#include "../sdk/sdk.hpp"

#include "../options.hpp"

void misc::nightmode()
{
	static bool old = false;

	// already applied, don't do it again
	if (options::misc::nightmode == old)
		return;

	static auto r_DrawSpecificStaticProp = g_cvar->FindVar("r_DrawSpecificStaticProp");
	static auto sv_skyname = g_cvar->FindVar("sv_skyname");
	
	r_DrawSpecificStaticProp->SetValue(options::misc::nightmode ? 0 : 1);

	static std::string old_sky;

	if (options::misc::nightmode)
		old_sky = sv_skyname->GetString();

	sv_skyname->SetValue(options::misc::nightmode ? "sky_csgo_night02" : old_sky.c_str());

	for (auto i = g_material_system->FirstMaterial(); i != g_material_system->InvalidMaterial(); i = g_material_system->NextMaterial(i))
	{
		IMaterial* mat = g_material_system->GetMaterial(i);
		if (!mat || mat->IsErrorMaterial())
			continue;
		
		if (strstr(mat->GetTextureGroupName(), "World") || strstr(mat->GetTextureGroupName(), "StaticProp"))
		{
			if (options::misc::nightmode)
				mat->ColorModulate(.25f, .25f, .25f);
			else
				mat->ColorModulate(1.f, 1.f, 1.f);
		}
		else if (strstr(mat->GetTextureGroupName(), "Model"))
		{
			if (options::misc::nightmode)
				mat->ColorModulate(.4f, .4f, .4f);
			else
				mat->ColorModulate(1.f, 1.f, 1.f);
		}
	}

	old = options::misc::nightmode;
}
