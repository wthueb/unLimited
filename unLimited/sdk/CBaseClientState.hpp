#pragma once

using uintptr_t = unsigned int;

class CBaseClientState
{
public:
	void ForceFullUpdate()
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x174) = -1;
	};
};

extern CBaseClientState* g_client_state;
