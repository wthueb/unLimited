#include "interfaces.hpp"

#include <Windows.h>
#include <string>

#include "sdk/sdk.hpp"

#include "utils.hpp"

CBaseClientState* g_client_state = nullptr;
CGlobalVarsBase* g_global_vars = nullptr;
CGlowObjectManager* g_glow_manager = nullptr;
CInput* g_input = nullptr;
IBaseClientDLL* g_client = nullptr;
IClientEntityList* g_entity_list = nullptr;
IClientMode* g_client_mode = nullptr;
ICvar* g_cvar = nullptr;
IEngineTrace* g_engine_trace = nullptr;
IGameEventManager2* g_game_event_manager = nullptr;
ILocalize* g_localize = nullptr;
IMaterialSystem* g_material_system = nullptr;
IPanel* g_panel = nullptr;
ISurface* g_surface = nullptr;
IVDebugOverlay* g_debug_overlay = nullptr;
IVEngineClient* g_engine = nullptr;
IVModelInfo* g_model_info = nullptr;
IVRenderView* g_render_view = nullptr;
IVModelRender* g_model_render = nullptr;

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
	g_glow_manager = *reinterpret_cast<CGlowObjectManager**>(utils::find_signature("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
	g_client = capture_interface<IBaseClientDLL>("client.dll", "VClient018");
	g_entity_list = capture_interface<IClientEntityList>("client.dll", "VClientEntityList003");
	g_cvar = capture_interface<ICvar>("vstdlib.dll", "VEngineCvar007");
	g_engine_trace = capture_interface<IEngineTrace>("engine.dll", "EngineTraceClient004");
	g_game_event_manager = capture_interface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002");
	g_localize = capture_interface<ILocalize>("localize.dll", "Localize_001");
	g_material_system = capture_interface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
	g_panel = capture_interface<IPanel>("vgui2.dll", "VGUI_Panel009");
	g_surface = capture_interface<ISurface>("vguimatsurface.dll", "VGUI_Surface031");
	g_debug_overlay = capture_interface<IVDebugOverlay>("engine.dll", "VDebugOverlay004");
	g_engine = capture_interface<IVEngineClient>("engine.dll", "VEngineClient014");
	g_model_info = capture_interface<IVModelInfo>("engine.dll", "VModelInfoClient004");
	g_model_render = capture_interface<IVModelRender>("engine.dll", "VEngineModel016");
	g_render_view = capture_interface<IVRenderView>("engine.dll", "VEngineRenderView014");

	g_client_state = **reinterpret_cast<CBaseClientState***>((*reinterpret_cast<uintptr_t**>(g_engine))[12] + 0x10);
	g_global_vars = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_client))[0] + 0x1B);
	g_input = *reinterpret_cast<CInput**>((*reinterpret_cast<uintptr_t**>(g_client))[15] + 0x1);
	g_client_mode = **reinterpret_cast<IClientMode***>((*reinterpret_cast<uintptr_t**>(g_client))[10] + 0x5);
}
