#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

#include "kit_parser.hpp"
#include "sdk/sdk.hpp"

struct item_t
{
	constexpr item_t(const char* model, const char* icon = nullptr)
		: model(model), icon(icon) {}

	const char* model;
	const char* icon;
};

struct wpn_name_t
{
	constexpr wpn_name_t(int definition_index, const char* name)
		: definition_index(definition_index), name(name) {}

	int definition_index = 0;
	const char* name;
};

struct quality_name_t
{
	constexpr quality_name_t(int index, const char* name)
		: index(index), name(name) {}

	int index = 0;
	const char* name;
};

static const std::map<int, item_t> model_info = {
	{ WEAPON_KNIFE, { "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
	{ WEAPON_KNIFE_T, { "models/weapons/v_knife_default_t.mdl", "knife_t" } },
	{ WEAPON_KNIFE_BAYONET, { "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
	{ WEAPON_KNIFE_FLIP, { "models/weapons/v_knife_flip.mdl", "knife_flip" } },
	{ WEAPON_KNIFE_GUT, { "models/weapons/v_knife_gut.mdl", "knife_gut" } },
	{ WEAPON_KNIFE_KARAMBIT, { "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
	{ WEAPON_KNIFE_M9_BAYONET, { "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
	{ WEAPON_KNIFE_TACTICAL, { "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
	{ WEAPON_KNIFE_FALCHION, { "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
	{ WEAPON_KNIFE_SURVIVAL_BOWIE, { "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
	{ WEAPON_KNIFE_BUTTERFLY, { "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
	{ WEAPON_KNIFE_PUSH, { "models/weapons/v_knife_push.mdl", "knife_push" } },
	{ GLOVE_STUDDED_BLOODHOUND, { "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl" } },
	{ GLOVE_T_SIDE, { "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl" } },
	{ GLOVE_CT_SIDE, { "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl" } },
	{ GLOVE_SPORTY, { "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl" } },
	{ GLOVE_SLICK, { "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl" } },
	{ GLOVE_LEATHER_WRAP, { "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl" } },
	{ GLOVE_MOTORCYCLE, { "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl" } },
	{ GLOVE_SPECIALIST, { "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl" } }
};

static const std::vector<wpn_name_t> knife_names = {
	{ 0, "default" },
	{ WEAPON_KNIFE_BAYONET, "bayonet" },
	{ WEAPON_KNIFE_FLIP, "flip knife" },
	{ WEAPON_KNIFE_GUT, "gut knife" },
	{ WEAPON_KNIFE_KARAMBIT, "karambit" },
	{ WEAPON_KNIFE_M9_BAYONET, "m9 bayonet" },
	{ WEAPON_KNIFE_TACTICAL, "huntsman knife" },
	{ WEAPON_KNIFE_FALCHION, "falchion knife" },
	{ WEAPON_KNIFE_SURVIVAL_BOWIE, "bowie knife" },
	{ WEAPON_KNIFE_BUTTERFLY, "butterfly knife" },
	{ WEAPON_KNIFE_PUSH, "shadow daggers" }
};

static const std::vector<wpn_name_t> glove_names = {
	{ 0, "default" },
	{ GLOVE_STUDDED_BLOODHOUND, "bloodhound" },
	{ GLOVE_T_SIDE, "default (terrorists)" },
	{ GLOVE_CT_SIDE, "default (counter-terrorists)" },
	{ GLOVE_SPORTY, "sporty" },
	{ GLOVE_SLICK, "slick" },
	{ GLOVE_LEATHER_WRAP, "handwrap" },
	{ GLOVE_MOTORCYCLE, "motorcycle" },
	{ GLOVE_SPECIALIST, "specialist" }
};

static const std::vector<wpn_name_t> weapon_names = {
	{ WEAPON_KNIFE, "knife" },
	{ GLOVE_T_SIDE, "glove" },
	{ 7, "ak-47" },
	{ 8, "aug" },
	{ 9, "awp" },
	{ 63, "cz75 auto" },
	{ 1, "desert eagle" },
	{ 2, "dual berettas" },
	{ 10, "famas" },
	{ 3, "five-seven" },
	{ 11, "g3sg1" },
	{ 13, "galil ar" },
	{ 4, "glock-18" },
	{ 14, "m249" },
	{ 60, "m4a1-s" },
	{ 16, "m4a4" },
	{ 17, "mac-10" },
	{ 27, "mag-7" },
	{ 33, "mp7" },
	{ 34, "mp9" },
	{ 28, "negev" },
	{ 35, "nova" },
	{ 32, "p2000" },
	{ 36, "p250" },
	{ 19, "p90" },
	{ 26, "pp-bizon" },
	{ 64, "r8 revolver" },
	{ 29, "sawed-off" },
	{ 38, "scar-20" },
	{ 40, "ssg 08" },
	{ 39, "sg 553" },
	{ 30, "tec-9" },
	{ 24, "ump-45" },
	{ 61, "usp-s" },
	{ 25, "xm1014" },
};

static const std::vector<quality_name_t> quality_names = {
	{ 0, "default" },
	{ 1, "genuine" },
	{ 2, "vintage" },
	{ 3, "unusual" },
	{ 5, "community" },
	{ 6, "developer" },
	{ 7, "self-made" },
	{ 8, "customized" },
	{ 9, "strange" },
	{ 10, "completed" },
	{ 12, "tournament" }
};

struct sticker_t
{
	int kit_index = 0;
	int kit_vector_index = 0;
	float wear = .00001f;
	float scale = 1.f;
	float rotation = 0.f;

	void update_values()
	{
		kit_vector_index = kit_vector_index < int(stickers.size()) ? kit_vector_index : stickers.size() - 1;
		kit_index = stickers.at(kit_vector_index).id;
	}

	void update_ids()
	{
		kit_vector_index = find_if(stickers.begin(), stickers.end(), [this](const kit_t& x)
		{
			return this->kit_index == x.id;
		}) - stickers.begin();
	}
};

struct econ_item_t
{
	char name[32] = "default";
	bool enabled = false;
	int definition_vector_index = 0;
	int definition_index = 1;
	int entity_quality_vector_index = 0;
	int entity_quality_index = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	int stat_trak = 0;
	float wear = .00001f;
	char custom_name[32] = "";
	std::array<sticker_t, 5> stickers;
	
	bool operator<(const econ_item_t& other) const
	{
		return strcmp(name, other.name) < 0;
	}

	void update_values()
	{
		definition_vector_index = definition_vector_index < int(weapon_names.size()) ? definition_vector_index : weapon_names.size() - 1;
		definition_index = weapon_names.at(definition_vector_index).definition_index;

		entity_quality_vector_index = entity_quality_vector_index < int(quality_names.size()) ? entity_quality_vector_index : quality_names.size() - 1;
		entity_quality_index = quality_names.at(entity_quality_vector_index).index;

		if (definition_index == GLOVE_T_SIDE)
		{
			paint_kit_vector_index = paint_kit_vector_index < int(glove_kits.size()) ? paint_kit_vector_index : glove_kits.size() - 1;
			paint_kit_index = glove_kits.at(paint_kit_vector_index).id;

			definition_override_vector_index = definition_override_vector_index < int(glove_names.size()) ? definition_override_vector_index : glove_names.size() - 1;
			definition_override_index = glove_names.at(definition_override_vector_index).definition_index;
		}
		else
		{
			paint_kit_vector_index = paint_kit_vector_index < int(weapon_kits.size()) ? paint_kit_vector_index : weapon_kits.size() - 1;
			paint_kit_index = weapon_kits.at(paint_kit_vector_index).id;

			definition_override_vector_index = definition_override_vector_index < int(knife_names.size()) ? definition_override_vector_index : knife_names.size() - 1;
			definition_override_index = knife_names.at(definition_override_vector_index).definition_index;
		}

		for (auto& sticker : stickers)
			sticker.update_values();
	}

	void update_ids()
	{
		definition_vector_index = find_if(weapon_names.begin(), weapon_names.end(), [this](const wpn_name_t& x)
		{
			return this->definition_index == x.definition_index;
		}) - weapon_names.begin();

		entity_quality_vector_index = find_if(quality_names.begin(), quality_names.end(), [this](const quality_name_t& x)
		{
			return this->entity_quality_index == x.index;
		}) - quality_names.begin();

		const auto& kit_set = definition_index == GLOVE_T_SIDE ? glove_kits : weapon_kits;

		paint_kit_vector_index = find_if(kit_set.begin(), kit_set.end(), [this](const kit_t& x)
		{
			return this->paint_kit_index == x.id;
		}) - kit_set.begin();

		const auto& override_set = definition_index == GLOVE_T_SIDE ? glove_names : knife_names;

		definition_override_vector_index = find_if(override_set.begin(), override_set.end(), [this](const wpn_name_t& x)
		{
			return this->definition_override_index == x.definition_index;
		}) - override_set.begin();

		for (auto& sticker : stickers)
			sticker.update_ids();
	}
};

namespace config
{
	void init();

	void save();
	void load();

	extern std::vector<econ_item_t> items;
}
