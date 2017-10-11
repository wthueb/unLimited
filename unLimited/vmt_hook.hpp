#pragma once

#include <Windows.h>
#include <cstring>

class vmt_hook
{
public:
	template <typename fn>
	vmt_hook(fn class_base)
	{
		m_class = reinterpret_cast<void***>(class_base);
		m_old_vmt = *m_class;

		size_t size = 0;
		while(m_old_vmt[size] && is_code_ptr(m_old_vmt[size]))
			++size;

		m_new_vmt = new void*[size];
		memcpy(m_new_vmt, m_old_vmt, sizeof(void*) * size);
		*m_class = m_new_vmt;
	}

	~vmt_hook()
	{
		*m_class = m_old_vmt;
		delete[] m_new_vmt;
	}

	template<typename fn>
	void* hook(fn hooked_fn, size_t index)
	{
		m_new_vmt[index] = reinterpret_cast<void*>(hooked_fn);
		return m_old_vmt[index];
	}

	template <typename fn = uintptr_t>
	fn get_original(int index)
	{
		return reinterpret_cast<fn>(m_old_vmt[index]);
	}

	void restore()
	{
		memcpy(*m_class, m_old_vmt, sizeof(m_new_vmt));
	}

private:
	bool is_code_ptr(void* ptr)
	{
		constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

		MEMORY_BASIC_INFORMATION out;
		VirtualQuery(ptr, &out, sizeof(out));

		return out.Type && !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS)) && out.Protect & protect_flags;
	}

	void*** m_class = nullptr;
	void** m_new_vmt = nullptr;
	void** m_old_vmt = nullptr;
};
