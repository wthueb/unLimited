#include "interfaces.hpp"

#include "sdk/sdk.hpp"

#include <Windows.h>
#include <string>

IBaseClientDLL* g_client = nullptr;
IVEngineClient* g_engine = nullptr;

template<typename t>
t* capture_interface(const std::string &module, const std::string &interface)
{
	using create_interface_fn = t*(*)(const char* name, int return_code);

	// technically we should cache the factories so we don't have to look up, say,
	// client.dll 10 times, but whatever, it's only ran once during initialization
	auto func = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(module.c_str()), "CreateInterface"));

	auto result = func(interface.c_str(), 0);

	if (!result)
		throw std::runtime_error("failed to get interface for: " + interface);

	return result;
}

void interfaces::init()
{
	g_client = capture_interface<IBaseClientDLL>("client.dll", "VClient008");
	g_engine = capture_interface<IVEngineClient>("engine.dll", "VEngineClient014");
}
