#include "hooks.hpp"

#include <d3d9.h>
#include <d3dx9.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <intrin.h>

#include "../gui.hpp"

static bool initialized = false;
static bool gui_open = false;

static HWND hwnd = nullptr;
static WNDPROC o_wndproc = nullptr;

// kind of hacky, but i don't want to have a header for this file or anything :/
// d3d_device gets initialized in hooks.cpp
extern uintptr_t d3d_device;

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

void gui_init()
{
	if (initialized)
		return;

	for (; !hwnd; hwnd = FindWindowA("Valve001", nullptr))
		Sleep(100);

	o_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongA(hwnd, GWLP_WNDPROC, LONG_PTR(hk_wndproc)));

	if (ImGui_ImplDX9_Init(hwnd, reinterpret_cast<IDirect3DDevice9*>(d3d_device)))
		initialized = true;

	gui::init();
}

void gui_shutdown()
{
	if (!initialized)
		return;

	SetWindowLongA(hwnd, GWLP_WNDPROC, LONG_PTR(o_wndproc));

	gui_open = false;

	auto cl_mouseenable = g_cvar->FindVar("cl_mouseenable");

	cl_mouseenable->SetValue(1);

	Sleep(2000);

	ImGui_ImplDX9_Shutdown();

	initialized = false;
}

HRESULT __stdcall hooks::hk_end_scene(IDirect3DDevice9* device)
{
	static const auto o_end_scene = d3d_device_hook->get_original<HRESULT(__stdcall*)(IDirect3DDevice9*)>(index::end_scene);

	// stupid double rendering
	static auto ret = _ReturnAddress();

	if (!initialized || _ReturnAddress() != ret)
		return o_end_scene(device);

	IDirect3DStateBlock9* state;
	device->CreateStateBlock(D3DSBT_PIXELSTATE, &state);

	if (gui_open)
	{
		ImGui::GetIO().MouseDrawCursor = true;

		ImGui_ImplDX9_NewFrame();

		gui::draw_gui();
		
		ImGui::Render();
	}

	state->Apply();
	state->Release();

	return o_end_scene(device);
}

HRESULT __stdcall hooks::hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
{
	static const auto o_reset = d3d_device_hook->get_original<HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)>(index::reset);

	if (!initialized)
		return o_reset(device, presentation_parameters);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto ret = o_reset(device, presentation_parameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

bool handle_input(UINT message, WPARAM w_param, LPARAM l_param)
{
	if (!initialized)
		return false;

	if (message == WM_KEYUP && w_param == VK_INSERT)
	{
		static auto cl_mouseenable = g_cvar->FindVar("cl_mouseenable");
		
		gui_open = !gui_open;

		cl_mouseenable->SetValue(!gui_open);
	}

	if (gui_open)
		return ImGui_ImplWin32_WndProcHandler(hwnd, message, w_param, l_param) == 0;

	return false;
}

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (handle_input(message, w_param, l_param))
		return true;

	return CallWindowProcA(o_wndproc, hwnd, message, w_param, l_param);
};
