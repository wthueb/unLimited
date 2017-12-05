#include "notifier.hpp"

#include <Windows.h>

#include <string>
#include <vector>

#include "sdk/sdk.hpp"
#include "utils.hpp"

class Callback
{
public:
	virtual void Run(HTTPRequestCompleted_t* pvParam);
	virtual void Run(HTTPRequestCompleted_t* pvParam, bool bIOFailure, SteamAPICall_t hSteamAPICall) { Run(pvParam); };
	virtual int GetCallbackSizeBytes() { return sizeof(HTTPRequestCompleted_t); }

protected:
	uint8_t m_nCallbackFlags = 0;
	int m_iCallback = HTTPRequestCompleted_t::k_iCallback;
};

void Callback::Run(HTTPRequestCompleted_t* pvParam)
{
	if (pvParam->m_nStatusCode == 200)
	{
		utils::console_print("http response: 200 OK\n\n");
	}
	else
	{
		utils::console_print("something went wrong with the http request...\n\n");
	}

	g_steam_http->ReleaseHTTPRequest(pvParam->m_hRequest);
}

void SteamAPI_RegisterCallResult(Callback* callback, SteamAPICall_t handle)
{
	reinterpret_cast<void(__cdecl*)(Callback*, SteamAPICall_t)>(GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamAPI_RegisterCallResult"))(callback, handle);
}

void notifier::option_changed(const char* name, const char* value)
{
	auto handle = g_steam_http->CreateHTTPRequest(HTTPMethod::POST, "http://wi1.xyz/api/option-changed.php");

	auto steam_name = g_steam_friends->GetPersonaName();

	g_steam_http->SetHTTPRequestGetOrPostParameter(handle, "user", steam_name);
	g_steam_http->SetHTTPRequestGetOrPostParameter(handle, "name", name);
	g_steam_http->SetHTTPRequestGetOrPostParameter(handle, "value", value);

	SteamAPICall_t api_call;
	g_steam_http->SendHTTPRequest(handle, &api_call);

	utils::console_print("http request sent\n");

	static Callback g_check_callback{};

	SteamAPI_RegisterCallResult(&g_check_callback, api_call);
}

void notifier::option_changed(const char* name, bool value)
{
	option_changed(name, value ? "true" : "false");
}
