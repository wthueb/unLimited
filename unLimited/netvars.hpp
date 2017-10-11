#pragma once

#include "sdk/Recv.hpp"

#include <vector>

class netvar_sys
{
	struct netvar_table
	{
		std::string               name;
		RecvProp*                 prop;
		uint32_t                  offset;
		std::vector<RecvProp*>    child_props;
		std::vector<netvar_table> child_tables;
	};

public:
	void init();

	void dump();
	void dump(std::ofstream &stream);

	uint32_t  get_offset(const std::string &table_name, const std::string &prop_name);
	RecvProp* get_prop(const std::string &table_name, const std::string &prop_name);

private:
	static netvar_table load_table(RecvTable* recv_table);
	static void         dump_table(std::ofstream &stream, const netvar_table &table, int indent);
	static uint32_t     get_offset(const netvar_table &table_name, const std::string &prop_name);
	static RecvProp*    get_prop(const netvar_table &table_name, const std::string &prop_name);

private:
	std::vector<netvar_table> tables;
};
