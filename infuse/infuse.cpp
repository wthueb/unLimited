// unLimited cheat loader
// made by wi1

#include "colors.hpp"
#include "update.hpp"

#include <Windows.h>
#include <TlHelp32.h>

#include <iostream>

#define exit cyan; std::cout << "press any key to exit...\n"; _getch(); return 0;

DWORD get_pid(const char* ProcessName)
{
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	// handle of the first program
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// iterate through all processes until process is found
	if (Process32First(snapshot, &pe) == TRUE)
	{
		while (Process32Next(snapshot, &pe) == TRUE)
		{
			if (strcmp(pe.szExeFile, ProcessName) == 0)
			{
				CloseHandle(snapshot);
				return pe.th32ProcessID;
			}
		}
	}

	return NULL;
}

BOOL inject_dll(DWORD pid, LPCSTR dll_path)
{
	// if process id or dll is null
	if (!pid || !dll_path)
		return FALSE;

	HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	// if process cannot be opened
	if (hproc == INVALID_HANDLE_VALUE)
		return FALSE;

	// same in every program, so no need to find where it is in other process
	LPVOID addr_to_loadlib = static_cast<LPVOID>(GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"));

	// allocate memory in other program for dll path to be stored
	LPVOID addr_to_dll = static_cast<LPVOID>(VirtualAllocEx(hproc, NULL, strlen(dll_path) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

	// write the path of the dll to that memory
	WriteProcessMemory(hproc, addr_to_dll, static_cast<LPVOID>(const_cast<char*>(dll_path + '\0')), strlen(dll_path) + 1, NULL);

	// load the dll in the target process by calling the address of the dll
	HANDLE remote_thread = CreateRemoteThread(hproc, NULL, NULL, static_cast<LPTHREAD_START_ROUTINE>(addr_to_loadlib),
		static_cast<LPVOID>(addr_to_dll), NULL, NULL);

	if (remote_thread != INVALID_HANDLE_VALUE)
	{
		// wait 10000 milliseconds for the dll to be loaded, else exit
		if (WaitForSingleObject(remote_thread, 10000) == WAIT_OBJECT_0)
		{
			// always close handles and unallocate memory
			VirtualFreeEx(hproc, addr_to_dll, strlen(dll_path) + 1, MEM_FREE);
			CloseHandle(remote_thread);
			CloseHandle(hproc);

			return TRUE;
		}
		else
		{
			// always close handles and unallocate memory
			VirtualFreeEx(hproc, addr_to_dll, strlen(dll_path) + 1, MEM_FREE);
			CloseHandle(remote_thread);
			CloseHandle(hproc);

			return FALSE;
		}
	}

	return FALSE;
}

void wait_for_exit();

int main()
{
	using namespace std;

	white;
	cout << "=====================================================================\n\n";
	cyan;
	cout << "                    unLimited csgo cheat loader\n";
	cout << "                            made by wi1\n\n";
	white;
	cout << "=====================================================================\n\n";

#ifndef _DEBUG
	if (!update())
#endif
	{
		red;
		cout << "unable to update. assuming current version works\n\n";
	}

	white;
	cout << "=====================================================================\n\n";

	cyan;
	cout << "attempting to load...\n\n";

	// default to csgo cause what else would they use a basic injector for anyways?
	string processname = "csgo";

	// add .exe if they did not
	if (processname.length() < 5 || processname.substr(processname.length() - 4) != ".exe")
		processname += ".exe";

	DWORD processid = get_pid(processname.c_str());

	// if process id cannot be found (not running/wrong name)
	if (!processid)
	{
		red;
		cout << "csgo.exe could not be found. is it running?\n\n";
		exit;
	}

	green;
	cout << processname << " has been found! process id: " << processid << "\n\n";

	string dllname;
	get_current_ver(dllname);

	// get absolute path of dll file
	char dllpath[MAX_PATH];
	GetFullPathNameA(dllname.c_str(), MAX_PATH, dllpath, NULL);

	// if path does not exist or is a directory
	if (GetFileAttributesA(dllpath) == INVALID_FILE_ATTRIBUTES || GetFileAttributesA(dllpath) & FILE_ATTRIBUTE_DIRECTORY)
	{
		red;
		cout << dllname << " is not in the same directory as this injector\n"
			           "did you rename it?\n\n";
		exit;
	}

	green;
	cout << dllname << " found! absolute path: " << dllpath << "\n\n";

	cyan;
	cout << "press any key to inject...\n";
	_getch();

	cyan;
	cout << "\nattempting to inject...\n\n";

	if (!inject_dll(processid, dllpath))
	{
		red;
		cout << "injection failed. make sure that cs:go is running and\n"
			    "that the .dll is in the same directory as this injector\n\n";
		exit;
	}
	
	white;
	cout << "=====================================================================\n\n";
	green;
	cout << "successfully injected " << dllname << " into " << processname << "!\n\n";
	white;
	cout << "=====================================================================\n\n";
	
	cyan;
	cout << "press any key to exit...\n\n";
	cout << "automatically exiting in 5 seconds...\n";

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
