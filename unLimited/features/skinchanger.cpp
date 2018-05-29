#include "features.hpp"

#include <array>
#include <string>
#include <vector>

#include "../config.hpp"
#include "../sdk/sdk.hpp"

enum class StickerAttributeType
{
	INDEX,
	WEAR,
	SCALE,
	ROTATION
};

static uint32_t s_econ_item_interface_wrapper_offset = 0u;

static void* oGetStickerAttributeBySlotIndexFloat = nullptr;

float __fastcall GetStickerAttributeBySlotIndexFloat(void* thisptr, void*, int slot, StickerAttributeType attribute, float unknown)
{
	auto item = reinterpret_cast<C_BaseAttributableItem*>(uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

	auto def_index = item->GetItemDefinitionIndex();

	auto config = config::get_by_definition_idx(def_index);

	if (config)
	{
		switch (attribute)
		{
		case StickerAttributeType::WEAR:
			return config->stickers[slot].wear;
		case StickerAttributeType::SCALE:
			return config->stickers[slot].scale;
		case StickerAttributeType::ROTATION:
			return config->stickers[slot].rotation;
		default:
			break;
		}
	}

	return reinterpret_cast<decltype(GetStickerAttributeBySlotIndexFloat)*>(oGetStickerAttributeBySlotIndexFloat)(thisptr, nullptr, slot, attribute, unknown);
}

static void* oGetStickerAttributeBySlotIndexInt = nullptr;

unsigned int __fastcall GetStickerAttributeBySlotIndexInt(void* thisptr, void*, int slot, StickerAttributeType attribute, unsigned unknown)
{
	auto item = reinterpret_cast<C_BaseAttributableItem*>(uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

	if (attribute == StickerAttributeType::INDEX)
	{
		auto def_index = item->GetItemDefinitionIndex();

		auto config = config::get_by_definition_idx(def_index);

		if (config)
			return config->stickers[slot].kit_index;
	}

	return reinterpret_cast<decltype(GetStickerAttributeBySlotIndexInt)*>(oGetStickerAttributeBySlotIndexInt)(thisptr, nullptr, slot, attribute, unknown);
}

bool is_code_ptr(void* ptr)
{
	constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

	MEMORY_BASIC_INFORMATION out;
	VirtualQuery(ptr, &out, sizeof(out));

	return out.Type && !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS)) && out.Protect & protect_flags;
}

void apply_sticker_hooks(C_BaseAttributableItem* item)
{
	if (!s_econ_item_interface_wrapper_offset)
		s_econ_item_interface_wrapper_offset = netvar_sys::get().get_offset("DT_EconEntity", "m_Item") + 0xC;
	
	void**& vmt = *reinterpret_cast<void***>(uintptr_t(item) + s_econ_item_interface_wrapper_offset);

	static void** hooked_vmt = nullptr;

	if (!hooked_vmt)
	{
		size_t size = 0;

		while (is_code_ptr(vmt[size]))
			++size;

		hooked_vmt = new void*[size];
		memcpy(hooked_vmt, vmt, size * sizeof(void*));

		oGetStickerAttributeBySlotIndexFloat = hooked_vmt[4];
		hooked_vmt[4] = reinterpret_cast<void*>(&GetStickerAttributeBySlotIndexFloat);

		oGetStickerAttributeBySlotIndexInt = hooked_vmt[5];
		hooked_vmt[5] = reinterpret_cast<void*>(&GetStickerAttributeBySlotIndexInt);
	}

	vmt = hooked_vmt;
}

void erase_override(int idx)
{
	if (model_info.count(idx))
	{
		auto& icon_override_map = config::icon_overrides;

		const auto& original_item = model_info.at(idx);

		if (original_item.icon && icon_override_map.count(original_item.icon))
			icon_override_map.erase(icon_override_map.at(original_item.icon));
	}
}

void apply_config(C_BaseAttributableItem* item, const econ_item_t* config, unsigned int xuid_low)
{
	item->GetItemIDHigh() = -1;

	item->GetAccountID() = xuid_low;

	item->GetEntityQuality() = config->entity_quality_index;

	if (config->custom_name[0])
		strcpy_s(item->GetCustomName(), config->custom_name);

	item->GetFallbackPaintKit() = config->paint_kit_index;

	item->GetFallbackSeed() = config->seed;

	if (config->stat_trak > 0)
		item->GetFallbackStatTrak() = config->stat_trak;

	item->GetFallbackWear() = config->wear;

	auto& definition_index = item->GetItemDefinitionIndex();

	auto& icon_override_map = config::icon_overrides;

	if (config->definition_override_index &&
		config->definition_override_index != definition_index &&
		model_info.count(config->definition_override_index))
	{
		unsigned int old_definition_index = definition_index;

		definition_index = config->definition_override_index;

		const auto& replacement_item = model_info.at(config->definition_override_index);

		item->SetModelIndex(g_model_info->GetModelIndex(replacement_item.model));
		item->GetClientNetworkable()->PreDataUpdate(0);

		if (old_definition_index && model_info.count(old_definition_index))
		{
			const auto& original_item = model_info.at(old_definition_index);

			if (original_item.icon && replacement_item.icon)
				icon_override_map[original_item.icon] = replacement_item.icon;
		}
	}
	else
	{
		erase_override(definition_index);
	}

	apply_sticker_hooks(item);
}

inline int random_sequence(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

// FIXMEW: use hashing for better performance
static int fix_animation(const char* model, const int sequence)
{
	if (!strcmp(model, "models/weapons/v_knife_butterfly.mdl"))
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default:
			return sequence + 1;
		}
	}
	else if (!strcmp(model, "models/weapons/v_knife_falchion_advanced.mdl"))
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	}
	else if (!strcmp(model, "models/weapons/v_knife_push.mdl"))
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return random_sequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	}
	else if (!strcmp(model, "models/weapons/v_knife_survival_bowie.mdl"))
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	}

	return sequence;
}

void skinchanger::apply_skins()
{
	auto local_idx = g_engine->GetLocalPlayer();

	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(local_idx));
	if (!localplayer)
		return;

	player_info_t player_info;
	if (!g_engine->GetPlayerInfo(local_idx, &player_info))
		return;

	auto& wearables = localplayer->GetWearables();

	static CHandle<C_BaseAttributableItem> glove_handle{};

	auto glove = wearables.Get();
	if (!glove)
	{
		auto last_glove = glove_handle.Get();
		if (last_glove)
		{
			wearables = glove_handle;
			glove = last_glove;
		}
	}

	if (!localplayer->IsAlive())
	{
		if (glove)
		{
			glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
			glove->GetClientNetworkable()->Release();
		}

		return;
	}

	auto glove_config = config::get_by_definition_idx(GLOVE_T_SIDE);

	if (glove_config && glove_config->definition_override_index)
	{
		if (!glove)
		{
			static CreateClientClassFn create_wearable_fn;
			
			if (!create_wearable_fn)
			{
				auto clazz = g_client->GetAllClasses();

				while (clazz->m_ClassID != ClassId_CEconWearable)
					clazz = clazz->m_pNext;

				create_wearable_fn = clazz->m_pCreateFn;
			}
			
			auto entry = g_entity_list->GetHighestEntityIndex() + 1;
			auto serial = rand() % 0x1000;

			create_wearable_fn(entry, serial);
			glove = reinterpret_cast<C_BaseAttributableItem*>(g_entity_list->GetClientEntity(entry));

			static auto set_abs_origin_fn =
				reinterpret_cast<void(__thiscall*)(void*, const Vector&)>(utils::find_signature("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1"));
			
			static const Vector new_pos = { 10000.f, 10000.f, 10000.f };

			set_abs_origin_fn(glove, new_pos);

			wearables = CHandle<C_BaseAttributableItem>(entry, serial);

			glove_handle = wearables;
		}

		// credit: beakers
		*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;

		apply_config(glove, glove_config, player_info.xuid_low);
	}

	auto weapons = localplayer->GetWeapons();
	
	for (auto i = 0; weapons[i].IsValid(); ++i)
	{
		auto weapon = weapons[i].Get();
		if (!weapon)
			continue;

		auto& definition_index = weapon->GetItemDefinitionIndex();

		if (auto active_conf = config::get_by_definition_idx(weapon->IsKnife() ? WEAPON_KNIFE : definition_index))
			apply_config(weapon, active_conf, player_info.xuid_low);
		else
			erase_override(definition_index);
	}

	auto viewmodel_handle = localplayer->GetViewModel();
	if (!viewmodel_handle.IsValid())
		return;

	auto viewmodel = viewmodel_handle.Get();
	if (!viewmodel)
		return;
	
	auto viewmodel_weapon_handle = viewmodel->GetWeapon();
	if (!viewmodel_weapon_handle.IsValid())
		return;

	auto viewmodel_weapon = viewmodel_weapon_handle.Get();
	if (!viewmodel_weapon)
		return;

	if (model_info.count(viewmodel_weapon->GetItemDefinitionIndex()))
	{
		auto& override_model = model_info.at(viewmodel_weapon->GetItemDefinitionIndex()).model;
		viewmodel->SetModelIndex(g_model_info->GetModelIndex(override_model));
	}
}

void skinchanger::fix_icons(IGameEvent* event)
{
	if (g_engine->GetPlayerForUserID(event->GetInt("attacker")) == g_engine->GetLocalPlayer())
		if (auto icon_override = config::get_icon_override(event->GetString("weapon")))
			event->SetString("weapon", icon_override);
}

void skinchanger::fix_anims()
{
	auto localplayer = static_cast<C_BasePlayer*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
	if (!localplayer || !localplayer->IsAlive())
		return;

	auto view_model = localplayer->GetViewModel().Get();
	if (!view_model)
		return;

	auto view_model_weapon = view_model->GetWeapon().Get();
	if (!view_model_weapon)
		return;

	static auto lastseq = -1;

	if (model_info.count(view_model_weapon->GetItemDefinitionIndex()))
	{
		const auto override_model = model_info.at(view_model_weapon->GetItemDefinitionIndex()).model;

		auto new_sequence = fix_animation(override_model, view_model->GetSequence());

		if (lastseq != view_model->GetSequence())
			view_model->SendViewModelMatchingSequence(new_sequence);
	}

	lastseq = view_model->GetSequence();
}
