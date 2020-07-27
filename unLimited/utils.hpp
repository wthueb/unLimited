#pragma once

#include <Windows.h>

#include <codecvt>
#include <string>

#include "netvars.hpp"

namespace utils
{
    extern bool should_unload;

    void unload();

    void attach_console();
    void detach_console();
    bool printf(const char* fmt, ...);
    char console_read_key();

    uint64_t find_signature(const std::string& module_name, const std::string& sig);
    void* get_export(const std::string& module_name, const std::string& export_name);
    
    std::wstring to_wstring(const std::string& str);

    extern HMODULE dll;
}

template<typename fn>
__forceinline static fn get_vfunc(void* base, int index)
{
    return reinterpret_cast<fn>((*reinterpret_cast<uintptr_t**>(base))[index]);
}

#define NETVAR(name, type, table, prop)                                 \
    type& name() const                                                  \
    {                                                                   \
        static auto offset = netvar_sys::get().get_offset(table, prop); \
        return *reinterpret_cast<type*>(uintptr_t(this) + offset);      \
    }

#define PNETVAR(name, type, table, prop)                                \
    type* name() const                                                  \
    {                                                                   \
        static auto offset = netvar_sys::get().get_offset(table, prop); \
        return reinterpret_cast<type*>(uintptr_t(this) + offset);       \
    }
