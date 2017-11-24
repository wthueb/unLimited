#pragma once

#include <array>
#include <string>
#include <vector>

struct sticker_t
{
	int kit_index = 0;
	int kit_vector_index = 0;
	float wear = FLT_MIN;
	float scale = 1.f;
	float rotation = 0.f;
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
	float wear = FLT_MIN;
	char custom_name[32] = "";
	std::array<sticker_t, 5> stickers;
	
	bool operator<(const econ_item_t& a)
	{
		return strcmp(name, a.name) < 0;
	}
};

namespace config
{
	void save();
	void load();

	std::vector<econ_item_t> items;
}
