#include "interfaces.hpp"

#include "sdk/sdk.hpp"

#include <Windows.h>
#include <string>

IVEngineClient* g_engine_client = nullptr;

template<typename t>
t* capture_interface(const std::string &module, const std::string &interface)
{
	using create_interface_fn = t*(*)(const char* name, int return_code);

	auto func = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(module.c_str()), "CreateInterface"));

	auto result = func(interface.c_str(), 0);

	if (!result)
		throw std::runtime_error("failed to get interface for: " + interface);

	return result;
}

void interfaces::init()
{
	g_engine_client = capture_interface<IVEngineClient>("engine.dll", "VEngineClient014");
}
