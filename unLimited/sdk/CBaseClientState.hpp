#pragma once

#include <cstdint>

class CBaseClientState
{
public:
	void ForceFullUpdate()
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x174) = -1;
	};
};
