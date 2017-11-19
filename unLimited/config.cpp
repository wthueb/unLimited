#include "config.hpp"

#include <fstream>
#include <json.hpp>

#include "sdk/sdk.hpp"

using json = nlohmann::json;

void to_json(json& j, const option& opt)
{
	j = json
	{
		{ "name", opt.name },
		//{ "value", *opt.value }
	};
}

void from_json(const json& j, option& opt)
{
	opt.name = j.at("name").get<std::string>();
	//*opt.value = j.at("value").get<>();
}

void config::save()
{
	std::ofstream{ "unLimited.json" } << std::setw(4) << json(options);
}

void config::load()
{
	try
	{
		json::parse(std::ifstream{ "unLimited.json" }).get<std::vector<option>>();
	}
	catch (const std::exception&)
	{
		// cfg doesn't exist, ignore
	}
}
