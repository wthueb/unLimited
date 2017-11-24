#include "config.hpp"

#include <fstream>
#include <json.hpp>

#include "sdk/sdk.hpp"

using json = nlohmann::json;

std::vector<econ_item_t> config::items;

void to_json(json& j, const sticker_t& sticker)
{
	j = json
	{
		{ "kit", sticker.kit_index },
		{ "wear", sticker.wear },
		{ "scale", sticker.scale },
		{ "rotation", sticker.rotation }
	};
}

void from_json(const json& j, sticker_t& sticker)
{
	sticker.kit_index = j.at("kit").get<int>();
	sticker.wear = j.at("wear").get<float>();
	sticker.scale = j.at("scale").get<float>();
	sticker.rotation = j.at("rotation").get<float>();
	sticker.update_ids();
}

void to_json(json& j, const econ_item_t& item)
{
	j = json
	{
		{ "name", item.name },
		{ "enabled", item.enabled },
		{ "definition_index", item.definition_index },
		{ "entity_quality_index", item.entity_quality_index },
		{ "paint_kit_index", item.paint_kit_index },
		{ "definition_override_index", item.definition_override_index },
		{ "seed", item.seed },
		{ "stat_trak", item.stat_trak },
		{ "wear", item.wear },
		{ "custom_name", item.custom_name },
		{ "stickers", item.stickers },
	};
}

void from_json(const json& j, econ_item_t& item)
{
	strcpy_s(item.name, j.at("name").get<std::string>().c_str());
	item.enabled = j.at("enabled").get<bool>();
	item.definition_index = j.at("definition_index").get<int>();
	item.entity_quality_index = j.at("entity_quality_index").get<int>();
	item.paint_kit_index = j.at("paint_kit_index").get<int>();
	item.definition_override_index = j.at("definition_override_index").get<int>();
	item.seed = j.at("seed").get<int>();
	item.stat_trak = j.at("stat_trak").get<int>();
	item.wear = j.at("wear").get<float>();
	strcpy_s(item.custom_name, j.at("custom_name").get<std::string>().c_str());
	auto stickers = j.at("stickers").get<std::vector<sticker_t>>();
	
	auto size = stickers.size() < item.stickers.size() ? stickers.size() : item.stickers.size();

	for (size_t i = 0; i < size; ++i)
		item.stickers[i] = stickers.at(i);

	item.update_ids();
}

void config::init()
{
	items.reserve(128);

	load();
}

void config::save()
{
	std::sort(items.begin(), items.end());
	std::ofstream("skin_config.json") << json(items);
}

void config::load()
{
	try
	{
		items = json::parse(std::ifstream("skin_config.json")).get<std::vector<econ_item_t>>();
		std::sort(items.begin(), items.end());
		g_client_state->ForceFullUpdate();
	}
	catch (const std::exception&) {}
}

econ_item_t* config::get_by_definition_idx(ItemDefinitionIndex idx)
{
	for (auto& item : items)
		if (item.enabled && item.definition_index == idx)
			return &item;

	return nullptr;
}
