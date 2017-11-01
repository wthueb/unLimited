#include "netvars.hpp"

#include <fstream>
#include <string>

#include "sdk/sdk.hpp"

void netvar_sys::init()
{
	tables.clear();

	for (auto client_class = g_client->GetAllClasses(); client_class; client_class = client_class->m_pNext)
	{
		if (client_class->m_pRecvTable)
			tables.emplace_back(load_table(client_class->m_pRecvTable));
	}
}

netvar_sys::netvar_table netvar_sys::load_table(RecvTable* recv_table)
{
	auto table = netvar_table{};

	table.offset = 0;
	table.name = recv_table->m_pNetTableName;

	for (auto i = 0; i < recv_table->m_nProps; ++i)
	{
		auto prop = &recv_table->m_pProps[i];

		if (!prop || isdigit(prop->m_pVarName[0]))
			continue;

		if (!strcmp("baseclass", prop->m_pVarName))
			continue;

		if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
		{
			table.child_tables.emplace_back(load_table(prop->m_pDataTable));
			table.child_tables.back().offset = prop->m_Offset;
			table.child_tables.back().prop = prop;
		}
		else
			table.child_props.emplace_back(prop);
	}

	return table;
}

void netvar_sys::dump()
{
	auto stream = std::ofstream("netvars.txt");

	dump(stream);
}

void netvar_sys::dump(std::ofstream &stream)
{
	for (const auto &table : tables)
	{
		if (table.child_props.empty() && table.child_tables.empty())
			continue;

		stream << table.name << '\n';
		dump_table(stream, table, 1);
		stream << '\n';
	}

	stream << std::endl;
}

void netvar_sys::dump_table(std::ofstream &stream, const netvar_table &table, int indent)
{
	char buffer[1024];

	for (const auto &prop : table.child_props)
	{
		sprintf_s(buffer, "%*c%*s: 0x%08X", indent * 4, ' ', -(50 - indent * 4), prop->m_pVarName, table.offset + prop->m_Offset);
		stream << buffer << '\n';
	}

	for (const auto &child : table.child_tables)
	{
		sprintf_s(buffer, "%*c%*s: 0x%08X", indent * 4, ' ', -(50 - indent * 4), child.prop->m_pVarName, table.offset + child.offset);
		stream << buffer << '\n';
		dump_table(stream, child, indent + 1);
	}
}

uint32_t netvar_sys::get_offset(const std::string &table_name, const std::string &prop_name)
{
	auto result = 0u;

	for (const auto &table : tables)
	{
		if (table.name == table_name)
		{
			result = get_offset(table, prop_name);
			if (result)
				return result;
		}
	}

	return 0;
}

uint32_t netvar_sys::get_offset(const netvar_table &table, const std::string &prop_name)
{
	for (const auto &prop : table.child_props) {
		if (!strncmp(prop->m_pVarName, prop_name.data(), prop_name.size()))
			return table.offset + prop->m_Offset;
	}

	for (const auto &child : table.child_tables) {
		auto prop_offset = get_offset(child, prop_name);
		if (prop_offset)
			return table.offset + prop_offset;
	}

	for (const auto& child : table.child_tables) {
		if (!strncmp(child.prop->m_pVarName, prop_name.data(), prop_name.size()))
			return table.offset + child.offset;
	}

	return 0;
}

RecvProp* netvar_sys::get_prop(const std::string &table_name, const std::string &prop_name)
{
	RecvProp* result = nullptr;

	for (const auto &table : tables)
	{
		if (table.name == table_name)
			result = get_prop(table, prop_name);
	}

	return result;
}

RecvProp* netvar_sys::get_prop(const netvar_table &table, const std::string &prop_name)
{
	for (const auto &prop : table.child_props)
	{
		if (!strncmp(prop->m_pVarName, prop_name.data(), prop_name.size()))
			return prop;
	}

	for (const auto &child : table.child_tables)
	{
		auto prop = get_prop(child, prop_name);
		if (prop)
			return prop;
	}

	for (const auto &child : table.child_tables)
	{
		if (!strncmp(child.prop->m_pVarName, prop_name.data(), prop_name.size()))
			return child.prop;
	}

	return nullptr;
}
