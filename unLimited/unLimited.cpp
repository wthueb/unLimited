#include <Windows.h>

#include "config.hpp"
#include "hooks/hooks.hpp"
#include "interfaces.hpp"
#include "kit_parser.hpp"
#include "options.hpp"
#include "utils.hpp"

extern void gui_init();

options_class options{};

void wait_for_unload()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_DELETE) & 1)
		{
			utils::should_unload = true;
			utils::console_print("unload called for by pressing delete\n\n");
		}

		if (utils::should_unload)
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

	utils::console_print("finding paint kits...\n\n");
	find_kits();
	utils::console_print("finished finding kits\n\n\n");

	utils::console_print("initializing gui...\n\n");
	gui_init();
	utils::console_print("finished initializing gui\n\n\n");

#ifdef _DEBUG
	utils::console_print("dumping netvars...\n\n");
	netvar_sys::get().dump();
	utils::console_print("finished dumping netvars...\n\n\n");
#endif

	config::init();

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
