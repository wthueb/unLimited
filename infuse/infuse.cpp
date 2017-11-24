// unLimited cheat loader
// made by wi1

#include "colors.hpp"
#include "update.hpp"

#include <Windows.h>
#include <TlHelp32.h>

#include <iostream>

#define exit cyan; std::cout << "press any key to exit...\n"; _getch(); return 0;

DWORD find_process(std::string process_name)
{
	white;
	std::cout << "=====================================================================\n\n";

	cyan;
	std::cout << "attempting to find " << process_name << "...\n\n";

	DWORD pid = 0ul;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(snapshot, &pe))
	{
		while (Process32Next(snapshot, &pe))
		{
			if (!strcmp(pe.szExeFile, process_name.c_str()))
			{
				CloseHandle(snapshot);
				pid = pe.th32ProcessID;
			}
		}
	}

	// if process id cannot be found (not running/wrong name)
	if (!pid)
	{
		red;
		std::cout << process_name << " could not be found. is it running?\n\n";
		exit;
	}

	green;
	std::cout << process_name << " has been found! process id: " << pid << "\n\n";
	white;
	std::cout << "=====================================================================\n\n";

	return pid;
}

bool inject_dll(DWORD pid, const char* dll_path)
{
	if (!pid || !dll_path)
		return false;

	HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (hproc == INVALID_HANDLE_VALUE)
		return false;

	void* addr_to_loadlib = static_cast<void*>(GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"));

	void* addr_to_dll_path = static_cast<void*>(VirtualAllocEx(hproc, 0, strlen(dll_path) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

	WriteProcessMemory(hproc, addr_to_dll_path, static_cast<void*>(const_cast<char*>(dll_path + '\0')), strlen(dll_path) + 1, nullptr);

	HANDLE remote_thread = CreateRemoteThread(hproc, 0, 0, static_cast<LPTHREAD_START_ROUTINE>(addr_to_loadlib),
		static_cast<void*>(addr_to_dll_path), 0, 0);

	if (remote_thread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(remote_thread, 10000) == WAIT_OBJECT_0)
		{
			VirtualFreeEx(hproc, addr_to_dll_path, strlen(dll_path) + 1, MEM_FREE);
			CloseHandle(remote_thread);
			CloseHandle(hproc);

			return true;
		}
		else
		{
			VirtualFreeEx(hproc, addr_to_dll_path, strlen(dll_path) + 1, MEM_FREE);
			CloseHandle(remote_thread);
			CloseHandle(hproc);

			return false;
		}
	}

	return false;
}

void wait_for_exit();

int main(int argc, char* argv[])
{
	white;
	std::cout << "=====================================================================\n\n";
	cyan;
	std::cout << "                    infuse, a csgo cheat loader                      \n";
	std::cout << "                            made by wi1                              \n";
	std::cout << "                              wi1.xyz                                \n\n";
	white;
	std::cout << "=====================================================================\n\n";

	std::string dll_name{};
	std::string dll_path{};

	// file dragged on, manual inject
	if (argc == 2)
	{
		dll_path = std::string{ argv[1] };

		dll_name = dll_path.substr(dll_path.find_last_of("\\") + 1);

		green;
		std::cout << "using " << dll_name << ", absolute path: " << dll_path << "\n\n";
	}
	else
	{
#ifndef _DEBUG
		if (!update_unLimited())
		{
			red;
			std::cout << "unable to update. assuming current version works\n\n";
		}
#endif

		dll_name = get_current_ver();

		char path[MAX_PATH];
		GetFullPathNameA(dll_name.c_str(), MAX_PATH, path, 0);

		if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES || GetFileAttributesA(path) & FILE_ATTRIBUTE_DIRECTORY)
		{
			red;
			std::cout << dll_name << " is not in the same directory as this injector\n"
				"did you rename it?\n\n";
			exit;
		}

		dll_path = std::string{ path };

		green;
		std::cout << "using " << dll_name << ", absolute path: " << dll_path << "\n\n";
	}

	std::string process_name{ "csgo.exe" };

	DWORD pid = find_process(process_name);

	cyan;
	std::cout << "press any key to inject...\n";
	_getch();

	cyan;
	std::cout << "\nattempting to inject...\n\n";

	if (!inject_dll(pid, dll_path.c_str()))
	{
		red;
		std::cout << "injection failed. make sure that cs:go is running and\n"
			    "that the .dll is in the same directory as this injector\n\n";
		exit;
	}
	
	white;
	std::cout << "=====================================================================\n\n";
	green;
	std::cout << "successfully injected " << dll_name << " into " << process_name << "!\n\n";
	white;
	std::cout << "=====================================================================\n\n";
	
	cyan;
	std::cout << "press any key to exit...\n\n";
	std::cout << "automatically exiting in 5 seconds...\n";

	CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(wait_for_exit), 0, 0, 0);

	_getch();

	return 0;
}

void wait_for_exit()
{
	Sleep(5000);
#ifdef exit
#undef exit
#endif
	exit(0);
}
