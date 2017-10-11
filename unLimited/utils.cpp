#include "utils.hpp"

#include "hooks/hooks.hpp"

#include <Psapi.h>
#include <sstream>

// gets set in DllMain when dll is attached
HMODULE utils::dll = NULL;

HANDLE out = NULL, old_out = NULL;
HANDLE err = NULL, old_err = NULL;
HANDLE in = NULL, old_in = NULL;

extern void gui_shutdown();

void utils::unload()
{
#ifdef _DEBUG
	detach_console();
#endif

	gui_shutdown();
	Sleep(100);

	hooks::unload();
	Sleep(500);

	FreeLibraryAndExitThread(dll, 0);
}

void utils::attach_console()
{
	old_out = GetStdHandle(STD_OUTPUT_HANDLE);
	old_err = GetStdHandle(STD_ERROR_HANDLE);
	old_in = GetStdHandle(STD_INPUT_HANDLE);

	AllocConsole() && AttachConsole(GetCurrentProcessId());

	out = GetStdHandle(STD_OUTPUT_HANDLE);
	err = GetStdHandle(STD_ERROR_HANDLE);
	in = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(out,
		ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

	SetConsoleMode(in,
		ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
		ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
}

void utils::detach_console()
{
	if (out && err && in) {
		FreeConsole();

		if (old_out)
			SetStdHandle(STD_OUTPUT_HANDLE, old_out);
		if (old_err)
			SetStdHandle(STD_ERROR_HANDLE, old_err);
		if (old_in)
			SetStdHandle(STD_INPUT_HANDLE, old_in);
	}
}

bool utils::console_print(const char* fmt, ...)
{
	if (!out)
		return false;

	char buf[1024];

	va_list va;
	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

	return !!WriteConsoleA(out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
}

char utils::console_read_key()
{
	if (!in)
		return false;

	auto key = char{ 0 };
	auto keysread = DWORD{ 0 };

	ReadConsoleA(in, &key, 1, &keysread, nullptr);

	return key;
}

// credits: learn_more
uint64_t utils::find_signature(const std::string &module, const std::string &signature)
{
#define INRANGE(x, a, b)  (x >= a && x <= b) 
#define GET_BITS(x)       (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GET_BYTE(x)       (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

	MODULEINFO mod_info;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module.c_str()), &mod_info, sizeof(MODULEINFO));

	auto startAddress = reinterpret_cast<DWORD>(mod_info.lpBaseOfDll);
	auto endAddress = startAddress + mod_info.SizeOfImage;

	const char* pat = signature.c_str();

	DWORD firstMatch = 0;
	for (auto cur = startAddress; cur < endAddress; ++cur)
	{
		if (!*pat)
			return firstMatch;

		if (*reinterpret_cast<PBYTE>(const_cast<char*>(pat)) == '\?' ||
			*reinterpret_cast<BYTE*>(cur) == GET_BYTE(pat))
		{
			if (!firstMatch)
				firstMatch = cur;

			if (!pat[2])
				return firstMatch;

			if (*reinterpret_cast<PWORD>(const_cast<char*>(pat)) == '\?\?' ||
				*reinterpret_cast<PBYTE>(const_cast<char*>(pat)) != '\?')
				pat += 3;
			else
				pat += 2;
		}
		else
		{
			pat = signature.c_str();
			firstMatch = 0;
		}
	}

	return NULL;
}

template<typename fn>
fn get_vfunc(void* base, int index)
{
	return *static_cast<fn**>(base)[index];
}
