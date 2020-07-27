#include "utils.hpp"

#include <Psapi.h>

#include <sstream>
#include <string>

#include "hooks/events.hpp"
#include "hooks/hooks.hpp"
#include "options.hpp"
#include "sdk/sdk.hpp"

// gets set in DllMain when dll is attached
HMODULE utils::dll = nullptr;

HANDLE out = nullptr, old_out = nullptr;
HANDLE err = nullptr, old_err = nullptr;
HANDLE in = nullptr, old_in = nullptr;

extern void gui_shutdown();

bool utils::should_unload = false;

void utils::unload()
{
#ifdef _DEBUG
	detach_console();
#endif
	
	options.unload();
	Sleep(100);
	
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

bool utils::printf(const char* fmt, ...)
{
	if (!out)
		return false;

	char buf[1024];

	va_list va;
	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

	return !!WriteConsoleA(out, buf, DWORD(strlen(buf)), nullptr, nullptr);
}

char utils::console_read_key()
{
	if (!in)
		return false;

	char key{};
	DWORD keys_read{};

	ReadConsoleA(in, &key, 1, &keys_read, nullptr);

	return key;
}

// credits: learn_more
uint64_t utils::find_signature(const std::string& module_name, const std::string& sig)
{
#define INRANGE(x, a, b)  (x >= a && x <= b) 
#define GET_BITS(x)       (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GET_BYTE(x)       (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

	MODULEINFO mod_info;
	K32GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module_name.c_str()), &mod_info, sizeof(MODULEINFO));

	auto start_address = reinterpret_cast<DWORD>(mod_info.lpBaseOfDll);
	auto end_address = start_address + mod_info.SizeOfImage;

	const char* pat = sig.c_str();

	uint64_t firstMatch;
	for (uint64_t cur = start_address; cur < end_address; ++cur)
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
			pat = sig.c_str();
			firstMatch = 0;
		}
	}

	return 0;
}

void* utils::get_export(const std::string& module_name, const std::string& export_name)
{
	HMODULE mod;
	while (!(mod = GetModuleHandleA(module_name.c_str())))
		Sleep(100);

	return reinterpret_cast<void*>(GetProcAddress(mod, export_name.c_str()));
}

std::wstring utils::to_wstring(const std::string& str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter{};

	try
	{
		return converter.from_bytes(str);
	}
	catch (std::range_error)
	{
		std::wostringstream s;
		s << str.c_str();
		return s.str();
	}
}
