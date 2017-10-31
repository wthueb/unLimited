#include "hooks.hpp"

#include "../gui.hpp"

#include <imgui_impl_dx9.h>

static bool initialized = false;
static bool gui_open = false;

static HWND hwnd{};
static WNDPROC o_wndproc{};

// kind of hacky, but i don't want to have a header for this file or anything :/
// d3d_device gets initialized in hooks.cpp
extern uintptr_t d3d_device;

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	gui_open = false;

	SetWindowLongA(hwnd, GWLP_WNDPROC, LONG_PTR(o_wndproc));

	ImGui_ImplDX9_Shutdown();

	initialized = false;
}

HRESULT __stdcall hooks::hk_end_scene(IDirect3DDevice9* d3d_device)
{
	static auto o_end_scene = d3d_device_hook->get_original<HRESULT(__stdcall*)(IDirect3DDevice9*)>(index::end_scene);

	static auto cl_mouseenable = g_cvar->FindVar("cl_mouseenable");

	static auto &style = ImGui::GetStyle();

	static float alpha = .01f;
	static float start_alpha = alpha;
	static float start_time = ImGui::GetTime();
	static bool old_state = false;

	if (gui_open != old_state)
	{
		start_time = ImGui::GetTime();
		old_state = gui_open;
	}

	if (gui_open)
	{
		if (cl_mouseenable->GetBool())
			cl_mouseenable->SetValue(0);

		ImGui::GetIO().MouseDrawCursor = true;

		ImGui_ImplDX9_NewFrame();

		alpha = std::clamp(start_alpha + .7f * (ImGui::GetTime() - start_time) / .5f, .01f, 1.f);

		style.Alpha = alpha;

		gui::draw_gui();

		ImGui::Render();
	}
	else
	{
		if (!cl_mouseenable->GetBool())
			cl_mouseenable->SetValue(1);

		if (alpha > .01f)
		{
			ImGui::GetIO().MouseDrawCursor = false;

			ImGui_ImplDX9_NewFrame();

			alpha = std::clamp(start_alpha - .7f * (ImGui::GetTime() - start_time) / .5f, .01f, 1.f);

			style.Alpha = alpha;

			gui::draw_gui();

			ImGui::Render();
		}
	}

	return o_end_scene(d3d_device);
}

HRESULT __stdcall hooks::hk_reset(IDirect3DDevice9* d3d_device, D3DPRESENT_PARAMETERS* presentation_parameters)
{
	static auto o_reset = d3d_device_hook->get_original<HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)>(index::reset);

	if (!initialized)
		return o_reset(d3d_device, presentation_parameters);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto ret = o_reset(d3d_device, presentation_parameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

bool handle_input(UINT message, WPARAM w_param, LPARAM l_param)
{
	if (!initialized)
		return false;

	if (message == WM_KEYUP && w_param == VK_INSERT)
		gui_open = !gui_open;

	if (gui_open)
		return ImGui_ImplWin32_WndProcHandler(hwnd, message, w_param, l_param) != 0;

	return false;
}

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (handle_input(message, w_param, l_param))
		return true;

	return CallWindowProcA(o_wndproc, hwnd, message, w_param, l_param);
};
