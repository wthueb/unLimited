#include "hooks.hpp"

#include "../features/features.hpp"
#include "../sdk/sdk.hpp"
#include "../vmt_hook.hpp"

bool __fastcall hooks::hk_dispatch_user_message(void* thisptr, void* edx, int type, unsigned int a3, unsigned int length, const void* msg_data)
{
    static const auto o_dispatch_user_message = client_hook->get_original<bool(__thiscall*)(void*, int, unsigned int, unsigned int, const void*)>(index::dispatch_user_message);

    /*if (type == CS_UM_SayText2)
    {
        bf_read read = bf_read(uintptr_t(msg_data));
        read.SetOffset(1);
        auto ent_index = read.ReadByte();
        read.Skip(3);
        auto msg_name = read.ReadString();
        auto player_name = read.ReadString();
        auto message = read.ReadString();
    }*/

    return o_dispatch_user_message(thisptr, type, a3, length, msg_data);
}
