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
IEngineSound* g_engine_sound = nullptr;
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

ISteamFriends* g_steam_friends = nullptr;
ISteamHTTP* g_steam_http = nullptr;
ISteamUser* g_steam_user = nullptr;

template<class T>
T* capture_interface(const std::string& module, const std::string& iface)
{
    using create_interface_fn = T*(*)(const char* name, int return_code);

    // technically we should cache the factories so we don't have to look up, say,
    // client.dll 10 times, but whatever, it's only ran once during initialization
    auto func = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(module.c_str()), "CreateInterface"));

    auto result = func(iface.c_str(), 0);

    if (!result)
        throw std::runtime_error("failed to get interface for: " + iface);

    return result;
}

void init_steam()
{
    ISteamClient* steam_client = reinterpret_cast<ISteamClient*>(reinterpret_cast<void*(__cdecl*)()>(GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamClient"))());

    const auto h_steam_user = reinterpret_cast<HSteamUser(__cdecl*)()>(GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamAPI_GetHSteamUser"))();
    const auto h_steam_pipe = reinterpret_cast<HSteamPipe(__cdecl*)()>(GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();

    g_steam_friends = steam_client->GetISteamFriends(h_steam_user, h_steam_pipe, "SteamFriends015");
    g_steam_http = steam_client->GetISteamHTTP(h_steam_user, h_steam_pipe, "STEAMHTTP_INTERFACE_VERSION002");
    g_steam_user = steam_client->GetISteamUser(h_steam_user, h_steam_pipe, "SteamUser019");
}

void interfaces::init()
{
    g_glow_manager = *reinterpret_cast<CGlowObjectManager**>(utils::find_signature("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
    g_input = *reinterpret_cast<CInput**>(utils::find_signature("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);

    g_client = capture_interface<IBaseClientDLL>("client_panorama.dll", "VClient018");
    g_entity_list = capture_interface<IClientEntityList>("client_panorama.dll", "VClientEntityList003");
    g_cvar = capture_interface<ICvar>("vstdlib.dll", "VEngineCvar007");
    g_engine_sound = capture_interface<IEngineSound>("engine.dll", "IEngineSoundClient003");
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
    g_client_mode = **reinterpret_cast<IClientMode***>((*reinterpret_cast<uintptr_t**>(g_client))[10] + 0x5);

    init_steam();

#ifdef _DEBUG
    utils::printf("g_client_state: 0x%08x\n", g_client_state);
    utils::printf("g_global_vars: 0x%08x\n", g_global_vars);
    utils::printf("g_glow_manager: 0x%08x\n", g_glow_manager);
    utils::printf("g_input: 0x%08x\n", g_input);
    utils::printf("g_client: 0x%08x\n", g_client);
    utils::printf("g_entity_list: 0x%08x\n", g_entity_list);
    utils::printf("g_client_mode: 0x%08x\n", g_client_mode);
    utils::printf("g_cvar: 0x%08x\n", g_cvar);
    utils::printf("g_engine_trace: 0x%08x\n", g_engine_trace);
    utils::printf("g_game_event_manager: 0x%08x\n", g_game_event_manager);
    utils::printf("g_localize: 0x%08x\n", g_localize);
    utils::printf("g_material_system: 0x%08x\n", g_material_system);
    utils::printf("g_panel: 0x%08x\n", g_panel);
    utils::printf("g_surface: 0x%08x\n", g_surface);
    utils::printf("g_debug_overlay: 0x%08x\n", g_debug_overlay);
    utils::printf("g_engine: 0x%08x\n", g_engine);
    utils::printf("g_model_info: 0x%08x\n", g_model_info);
    utils::printf("g_render_view: 0x%08x\n", g_render_view);
    utils::printf("g_model_render: 0x%08x\n\n", g_model_render);

    utils::printf("g_steam_friends: 0x%08x\n", g_steam_friends);
    utils::printf("g_steam_http: 0x%08x\n", g_steam_http);
    utils::printf("g_steam_user: 0x%08x\n\n", g_steam_user);
#endif
}
