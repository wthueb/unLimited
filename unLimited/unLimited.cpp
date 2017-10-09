#include <Windows.h>

#include "hooks/hooks.hpp"
#include "interfaces.hpp"
#include "utils.hpp"

void wait_for_unload()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_DELETE))
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

	utils::console_print("successfully attached dll to process\n\n");

	utils::console_print("initializing interfaces...\n\n");
	interfaces::init();
	utils::console_print("\ndone initializing interfaces\n\n");

	utils::console_print("initializing hooks...\n\n");
	hooks::init();
	utils::console_print("\ndone initializing hooks\n\n");

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
		if (!reserved)
			utils::unload();
		return TRUE;
		break;
	}

	return FALSE;
}