#include <Windows.h>

#include "hooks/hooks.hpp"
#include "interfaces.hpp"
#include "utils.hpp"

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 2

extern void gui_init();

void wait_for_unload()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_DELETE) & 1)
			utils::unload();

		Sleep(500);
	}
}

void attach(HMODULE dll)
{
	utils::dll = dll;

#ifdef _DEBUG
	utils::attach_console();
#endif

	utils::console_print("successfully attached dll to process\n\n\n");

	utils::console_print("initializing interfaces...\n\n");
	interfaces::init();
	utils::console_print("finished initializing interfaces\n\n\n");

	utils::console_print("initializing hooks...\n\n");
	hooks::init();
	utils::console_print("finished initializing hooks\n\n\n");

	utils::console_print("initializing gui...\n\n");
	gui_init();
	utils::console_print("finished initializing gui\n\n\n");

#ifdef _DEBUG
	utils::console_print("dumping netvars...\n\n");
	netvar_sys::get().dump();
	utils::console_print("finished dumping netvars...\n\n\n");
#endif

	wait_for_unload();
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(dll);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(attach), dll, NULL, nullptr);
		return TRUE;
		break;

	case DLL_PROCESS_DETACH:
		return TRUE;
		break;
	}

	return FALSE;
}
