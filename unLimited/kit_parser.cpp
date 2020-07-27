// credit to namazso - https://github.com/namazso/nSkinz

#include "kit_parser.hpp"

#include <algorithm>
#include <codecvt>

#include "sdk/sdk.hpp"
#include "utils.hpp"

std::vector<kit_t> weapon_kits;
std::vector<kit_t> glove_kits;
std::vector<kit_t> stickers;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct Node_t
{
    int previous_id;        //0x0000
    int next_id;            //0x0004
    void* _unknown_ptr;     //0x0008
    int _unknown;           //0x000C
    Key key;                //0x0010
    Value value;            //0x0014
};

template <typename Key, typename Value>
struct Head_t
{
    Node_t<Key, Value>* memory;     //0x0000
    int allocation_count;           //0x0004
    int grow_size;                  //0x0008
    int start_element;              //0x000C
    int next_available;             //0x0010
    int _unknown;                   //0x0014
    int last_element;               //0x0018
}; //Size=0x001C

// could use CUtlString but this is just easier and CUtlString isn't needed anywhere else
struct String_t
{
    char* buffer;   //0x0000
    int capacity;   //0x0004
    int grow_size;  //0x0008
    int length;     //0x000C
}; //Size=0x0010

struct CPaintKit
{
    int id;                     //0x0000

    String_t name;              //0x0004
    String_t description;       //0x0014
    String_t item_name;         //0x0024
    String_t material_name;     //0x0034
    String_t image_inventory;   //0x0044

    char pad_0x0054[0x8C];      //0x0054
}; //Size=0x00E0

struct CStickerKit
{
    int id;

    int item_rarity;

    String_t name;
    String_t description;
    String_t item_name;
    String_t material_name;
    String_t image_inventory;

    int tournament_event_id;
    int tournament_team_id;
    int tournament_player_id;
    bool is_custom_sticker_material;

    float rotate_end;
    float rotate_start;

    float scale_min;
    float scale_max;

    float wear_min;
    float wear_max;

    String_t image_inventory2;
    String_t image_inventory_large;

    uint32_t pad0[4];
};

void find_kits()
{
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    
    // search the relative calls

    // call    ItemSystem
    // push    dword ptr [esi+0Ch]
    // lea     ecx, [eax+4]
    // call    CEconItemSchema::GetPaintKitDefinition

    auto sig_address = utils::find_signature("client_panorama.dll", "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");

    // skip the opcode, read rel32 address
    auto item_system_offset = *reinterpret_cast<int32_t*>(sig_address + 1);

    // add the offset to the end of the instruction
    auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem*(*)()>(sig_address + 5 + item_system_offset);

    // skip vtable, first member variable of ItemSystem is ItemSchema
    auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(item_system_fn()) + sizeof(void*));

    const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(utils::get_export("vstdlib.dll", "V_UCS2ToUTF8"));

    // dump paint kits
    {
        // skip the instructions between, skip the opcode, read rel32 address
        auto get_paint_kit_definition_offset = *reinterpret_cast<int32_t*>(sig_address + 11 + 1);

        // add the offset to the end of the instruction
        auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);

        // the last offset is head_element, we need that

        // push    ebp
        // mov     ebp, esp
        // sub     esp, 0Ch
        // mov     eax, [ecx+298h]

        // skip instructions, skip opcode, read offset
        auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

        // calculate head base from start_element's offset
        auto head_offset = start_element_offset - 12;

        auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(uintptr_t(item_schema) + head_offset);

        for (auto i = 0; i <= map_head->last_element; ++i)
        {
            const auto paint_kit = map_head->memory[i].value;

            if (paint_kit->id == 9001)
                continue;

            const auto wide_name = g_localize->Find(paint_kit->item_name.buffer + 1);
            
            char name[256];
            V_UCS2ToUTF8(wide_name, name, sizeof(name));

            if (paint_kit->id < 10000)
                weapon_kits.push_back(kit_t{ paint_kit->id, name });
            else
                glove_kits.push_back(kit_t{ paint_kit->id, name });
        }

        std::sort(weapon_kits.begin(), weapon_kits.end());
        std::sort(glove_kits.begin(), glove_kits.end());
    }

    // dump sticker kits
    {
        auto sticker_sig = utils::find_signature("client_panorama.dll", "53 8D 48 04 E8 ? ? ? ? 8B 4D 10") + 4;

        // skip the opcode, read rel32 address
        auto get_sticker_kit_definition_offset = *reinterpret_cast<intptr_t*>(sticker_sig + 1);

        // add the offset to the end of the instruction
        auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

        // the last offset is head_element, we need that

        //  push    ebp
        //  mov     ebp, esp
        //  push    ebx
        //  push    esi
        //  push    edi
        //  mov     edi, ecx
        //  mov     eax, [edi + 2BCh]

        // skip instructions, skip opcode, read offset
        auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

        // calculate head base from start_element's offset
        auto head_offset = start_element_offset - 12;

        auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(uintptr_t(item_schema) + head_offset);

        for (int i = 0; i <= map_head->last_element; ++i)
        {
            auto sticker_kit = map_head->memory[i].value;

            char sticker_name_if_valve_fucked_up_their_translations[64];

            auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;

            if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
            {
                strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
                strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
                sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
            }

            const auto wide_name = g_localize->Find(sticker_name_ptr);

            char name[256];
            V_UCS2ToUTF8(wide_name, name, sizeof(name));

            stickers.push_back(kit_t{ sticker_kit->id, name });
        }

        std::sort(stickers.begin(), stickers.end());

        stickers.insert(stickers.begin(), kit_t{ 0, "None" });
    }
}
