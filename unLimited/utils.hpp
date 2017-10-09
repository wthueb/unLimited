#pragma once

#include <Windows.h>
#include <codecvt>

namespace utils
{
	void unload();

	void attach_console();
	void detach_console();
	bool console_print(const char* fmt, ...);
	char console_read_key();

	uint64_t find_signature(const std::string &szModule, const std::string &szSignature);

	template<typename fn>
	fn get_vfunc(void* base, int index);

	extern HMODULE dll;
}
