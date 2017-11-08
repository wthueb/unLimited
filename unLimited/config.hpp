#pragma once

#include <string>
#include <vector>

struct option
{
	std::string name;
	void* value;
};

namespace config
{
	void save();
	void load();

	std::vector<option> options;
}
