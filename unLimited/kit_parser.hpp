#pragma once

#include <vector>

struct kit_t
{
	int id;
	std::string name;

	bool operator<(const kit_t& other) const
	{
		return name < other.name;
	}
};

extern std::vector<kit_t> weapon_kits;
extern std::vector<kit_t> glove_kits;
extern std::vector<kit_t> stickers;

extern void find_kits();
