#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <cmath>
#include <stdexcept>

#include <imgui.h>
#include <imgui_impl_dx9.h>

#include "../unLimited/gui.hpp"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void init(HWND hwnd, LPDIRECT3DDEVICE9 device)
{
	static bool initialized = false;

	if (initialized)
		return;

	if (ImGui_ImplDX9_Init(hwnd, reinterpret_cast<IDirect3DDevice9*>(device)))
		initialized = true;

	gui::setup_style();
}

void render(LPDIRECT3DDEVICE9 device)
{
	device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	device->BeginScene();

	ImGui_ImplDX9_NewFrame();

	gui::draw_gui();

	ImGui::Render();

	device->EndScene();
	device->Present(0, 0, 0, 0);
}

class win32_window
{
public:
	win32_window(HINSTANCE instance)
	{
		ZeroMemory(&wc, sizeof(wc));
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = instance;
		wc.lpszClassName = "D3D";
		wc.hCursor = LoadCursor(0, IDC_ARROW);

		RegisterClass(&wc);

		RECT wnd_size = { 0, 0, 900, 550 };

		DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

		hwnd = CreateWindowA("D3D", "unLimited GUI", style, 0, 100, wnd_size.right, wnd_size.bottom, GetDesktopWindow(), 0, wc.hInstance, 0);
	}

	~win32_window()
	{
		if (wc.hInstance)
		{
			UnregisterClass("D3D", wc.hInstance);
		}
	}

	HWND hwnd;

private:
	WNDCLASS wc;
};

class d3d_device
{
public:
	d3d_device(HWND hwnd)
		: D3D(nullptr),
		Device(nullptr)
	{
		if (!(D3D = Direct3DCreate9(D3D_SDK_VERSION)))
		{
			throw std::runtime_error("can't create Direct3D9");
		}

		RECT rect;
		GetClientRect(hwnd, &rect);

		D3DPRESENT_PARAMETERS pp;
		ZeroMemory(&pp, sizeof(pp));
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		pp.Windowed = TRUE;
		pp.BackBufferWidth = rect.right;
		pp.BackBufferHeight = rect.bottom;

		if (FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &Device)))
		{
			D3D->Release();

			throw std::runtime_error("can't create device");
		}

		D3DXMATRIX projection;
		D3DXMatrixPerspectiveFovLH(&projection, D3DXToRadian(45.0f), 584.0f / 562.0f, 1.0f, 100.0f);
		Device->SetTransform(D3DTS_PROJECTION, &projection);
	}
	~d3d_device()
	{
		if (Device)
		{
			Device->Release();
		}
		if (D3D)
		{
			D3D->Release();
		}
	}

	LPDIRECT3DDEVICE9 Device;

private:
	LPDIRECT3D9 D3D;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
	win32_window window(instance);

	d3d_device device(window.hwnd);

	init(window.hwnd, device.Device);

	ShowWindow(window.hwnd, SW_SHOWDEFAULT);
	UpdateWindow(window.hwnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	
	while (true)
	{
		if (!IsWindowVisible(window.hwnd))
			break;

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//let the OSHGui handle the input
			if (!ImGui_ImplWin32_WndProcHandler(window.hwnd, msg.message, msg.wParam, msg.lParam))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
			render(device.Device);
	}
}
