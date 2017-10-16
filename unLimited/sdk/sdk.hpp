#pragma once

#include "CBaseClientState.hpp"
#include "CGlobalVarsBase.hpp"
#include "CGlowObjectManager.hpp"
#include "IBaseClientDll.hpp"
#include "IClientEntity.hpp"
#include "IClientEntityList.hpp"
#include "IClientMode.hpp"
#include "ICvar.hpp"
#include "IEngineTrace.hpp"
#include "IGameEventManager2.hpp"
#include "ILocalize.hpp"
#include "IPanel.hpp"
#include "ISurface.hpp"
#include "IVDebugOverlay.hpp"
#include "IVEngineClient.hpp"
#include "IVModelInfo.hpp"

extern CBaseClientState* g_client_state;
extern CGlobalVarsBase* g_global_vars;
extern CGlowObjectManager* g_glow_manager;
extern IBaseClientDLL* g_client;
extern IClientEntityList* g_entity_list;
extern IClientMode* g_client_mode;
extern ICvar* g_cvar;
extern IEngineTrace* g_engine_trace;
extern IGameEventManager2* g_game_event_manager;
extern ILocalize* g_localize;
extern IPanel* g_panel;
extern ISurface* g_surface;
extern IVDebugOverlay* g_debug_overlay;
extern IVEngineClient* g_engine;
extern IVModelInfo* g_model_info;
