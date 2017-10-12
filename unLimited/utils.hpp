#pragma once

#include "netvars.hpp"

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

	extern HMODULE dll;
}

template<typename fn>
__forceinline static fn get_vfunc(void* base, int index)
{
	return reinterpret_cast<fn>((*reinterpret_cast<uintptr_t**>(base))[index]);
}

#define NETVAR(type, name, table, netvar)                                      \
    type& name##() const {                                                     \
        static uint32_t _##name = netvar_sys::get().get_offset(table, netvar); \
        return *reinterpret_cast<type*>(uintptr_t(this) + _##name);            \
    }

#define PNETVAR(type, name, table, netvar)                                     \
    type* name##() const {                                                     \
        static uint32_t _##name = netvar_sys::get().get_offset(table, netvar); \
        return reinterpret_cast<type*>(uintptr_t(this) + _##name);             \
    }
