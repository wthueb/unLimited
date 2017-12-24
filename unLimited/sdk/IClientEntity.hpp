#pragma once

#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientThinkable.hpp"
#include "IClientUnknown.hpp"

#include "CHandle.hpp"
#include "VMatrix.hpp"

#include "../utils.hpp"

#define MAX_SHOOT_SOUNDS 16
#define MAX_WEAPON_STRING 80
#define MAX_WEAPON_PREFIX 16
#define MAX_WEAPON_AMMO_NAME 32

enum ItemDefinitionIndex
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034
};

enum Bone
{
	BONE_INVALID = -1,
	BONE_PELVIS,
	LEAN_ROOT,
	CAM_DRIVER,
	BONE_HIP,
	BONE_LOWER_SPINAL_COLUMN,
	BONE_MIDDLE_SPINAL_COLUMN,
	BONE_UPPER_SPINAL_COLUMN,
	BONE_NECK,
	BONE_HEAD,
	BONE_MAX_BONES
};

enum LifeState
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

enum Team
{
	TEAM_UNASSIGNED = 0,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST,
};

enum Flags
{
	FL_ONGROUND = (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 2),
	FL_ONTRAIN = (1 << 3),
	FL_INRAIN = (1 << 4),
	FL_FROZEN = (1 << 5),
	FL_ATCONTROLS = (1 << 6),
	FL_CLIENT = (1 << 7),
	FL_FAKECLIENT = (1 << 8)
};

enum MoveType
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

enum ObserverMode
{
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM,
	OBS_MODE_FREEZECAM,
	OBS_MODE_FIXED,
	OBS_MODE_IN_EYE,
	OBS_MODE_CHASE,
	OBS_MODE_ROAMING
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum WeaponSound
{
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	FAST_RELOAD,

	// Add new shoot sound types here
	REVERSE_THE_NEW_SOUND,

	NUM_SHOOT_SOUND_TYPES,
};

enum Sequence
{
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_BOWIE_IDLE1 = 1,
};

struct SpatializationInfo_t;

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void Release() = 0;
};

class C_BaseEntity : public IClientEntity
{
public:
	NETVAR(GetSimulationTime, float, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(GetOrigin, Vector, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(GetSpotted, bool, "DT_BaseEntity", "m_bSpotted");
	NETVAR(GetModelIndex, int, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(GetTeam, Team, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(GetOwnerEntity, CHandle<C_BaseEntity>, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(GetShouldGlow, bool, "DT_DynamicProp", "m_bShouldGlow");
	NETVAR(GetSequence, Sequence, "DT_BaseAnimating", "m_nSequence");

	Vector GetAbsOrigin()
	{
		return get_vfunc<Vector(__thiscall*)(void*)>(this, 10)(this);
	}

	void SetModelIndex(int index)
	{
		return get_vfunc<void(__thiscall*)(void*, int)>(this, 75)(this, index);
	}

	bool IsWeapon()
	{
		return get_vfunc<bool(__thiscall*)(void*)>(this, 160)(this);
	}

	bool IsPlayer()
	{
		return this->GetClientClass()->m_ClassID == ClassId_CCSPlayer;
	}
};

using cstr_32 = char[32];

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(GetItemDefinitionIndex, int, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(GetFallbackPaintKit, int, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(GetFallbackSeed, int, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(GetFallbackWear, float, "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(GetFallbackStatTrak, int, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(GetItemIDHigh, int, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(GetAccountID, int, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(GetEntityQuality, int, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(GetCustomName, cstr_32, "DT_BaseAttributableItem", "m_szCustomName");
};

class CHudTexture;

class KeyValues
{
public:
	KeyValues(const char* name)
	{
		static auto func = reinterpret_cast<void(__thiscall*)(void*, const char*)>
			(utils::find_signature("client.dll", "55 8B EC 51 33 C0 C7 45 ? ? ? ? ? 56 8B F1 81 26 ? ? ? ? C6 46 03 00 89 46 10 89 46 18 89 46 14 89 46 1C 89 46 04 89 46 08 89 46 0C FF 15 ? ? ? ? 6A 01 FF 75 08 8D 4D FC 8B 10 51 8B C8 FF 52 24 8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03"));
		
		func(this, name);
	}

	void LoadFromBuffer(const char* resource_name, const char* buffer, void* file_system = nullptr, const char* path_id = nullptr, void* evaluate_symbol_proc = nullptr)
	{
		static auto func = reinterpret_cast<void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*)>
			(utils::find_signature("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));

		func(this, resource_name, buffer, file_system, path_id, evaluate_symbol_proc);
	}
};

class FileWeaponInfo_t
{
public:
	FileWeaponInfo_t() {}

	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse(KeyValues *pKeyValuesData, const char *szWeaponName) = 0;

	bool bParsedScript;
	bool bLoadedHudElements;

	char szClassName[MAX_WEAPON_STRING];
	char szPrintName[MAX_WEAPON_STRING];

	char szViewModel[MAX_WEAPON_STRING];
	char szWorldModel[MAX_WEAPON_STRING];
	char szAmmo1[MAX_WEAPON_AMMO_NAME];
	char szWorldDroppedModel[MAX_WEAPON_STRING];
	char szAnimationPrefix[MAX_WEAPON_PREFIX];
	int iSlot;
	int iPosition;
	int iMaxClip1;
	int iMaxClip2;
	int iDefaultClip1;
	int iDefaultClip2;
	int iWeight;
	int iRumbleEffect;
	bool bAutoSwitchTo;
	bool bAutoSwitchFrom;
	int iFlags;
	char szAmmo2[MAX_WEAPON_AMMO_NAME];
	char szAIAddOn[MAX_WEAPON_STRING];

	// Sound blocks
	char aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

	int iAmmoType;
	int iAmmo2Type;
	bool m_bMeleeWeapon;

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool m_bBuiltRightHanded;
	bool m_bAllowFlipping;

	// Sprite data, read from the data file
	int iSpriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	NETVAR(GetOwner, int, "DT_BaseCombatWeapon", "m_hOwner");
	NETVAR(GetAmmo, int, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(GetNextPrimaryAttack, float, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(GetInReload, bool, "DT_BaseCombatWeapon", "m_bInReload");
	NETVAR(GetAccuracyPenalty, float, "DT_BaseCombatWeapon", "m_fAccuracyPenalty");

	bool IsPistol()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == WEAPON_DEAGLE || weaponid == WEAPON_ELITE ||
			weaponid == WEAPON_FIVESEVEN || weaponid == WEAPON_P250 ||
			weaponid == WEAPON_GLOCK || weaponid == WEAPON_HKP2000 ||
			weaponid == WEAPON_CZ75A || weaponid == WEAPON_USP_SILENCER ||
			weaponid == WEAPON_TEC9 || weaponid == WEAPON_REVOLVER;
	}

	bool IsShotgun()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == WEAPON_XM1014 || weaponid == WEAPON_NOVA ||
			weaponid == WEAPON_SAWEDOFF || weaponid == WEAPON_MAG7;
	}

	bool IsSniper()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == WEAPON_AWP || weaponid == WEAPON_SCAR20 ||
			weaponid == WEAPON_G3SG1 || weaponid == WEAPON_SSG08;
	}

	bool IsNade()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == WEAPON_FLASHBANG || weaponid == WEAPON_HEGRENADE ||
			weaponid == WEAPON_SMOKEGRENADE || weaponid == WEAPON_MOLOTOV ||
			weaponid == WEAPON_DECOY || weaponid == WEAPON_INCGRENADE;
	}

	bool IsKnife()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == WEAPON_KNIFE || weaponid == WEAPON_KNIFE_T ||
			weaponid >= 500;
	}

	bool IsBomb()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == WEAPON_C4;
	}
};

class C_BaseCSGrenade : public C_BaseCombatWeapon
{
public:
	NETVAR(GetPinPulled, bool, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(GetThrowTime, float, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(GetThrowStrength, float, "DT_BaseCSGrenade", "m_flThrowStrength");
};

class C_BasePlayer;

class C_BaseViewModel : public C_BaseEntity
{
public:
	NETVAR(GetWeapon, CHandle<C_BaseCombatWeapon>, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(GetOwner, CHandle<C_BasePlayer>, "DT_BaseViewModel", "m_hOwner");

	void SendViewModelMatchingSequence(int sequence) {
		using ofunc = void(__thiscall*)(void*, int);
		return get_vfunc<ofunc>(this, 241)(this, sequence);
	}
};

class C_BasePlayer : public C_BaseEntity
{
public:
	NETVAR(GetFlags, Flags, "DT_BasePlayer", "m_fFlags");
	NETVAR(GetHealth, int, "DT_BasePlayer", "m_iHealth");
	NETVAR(GetLifeState, LifeState, "DT_BasePlayer", "m_lifeState");
	NETVAR(GetObserverTarget, CHandle<C_BasePlayer>, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(GetViewModel, CHandle<C_BaseViewModel>, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(GetViewOffset, Vector, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(GetTickBase, int, "DT_BasePlayer", "m_nTickBase");
	NETVAR(GetVelocity, Vector, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(GetViewPunch, QAngle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(GetAimPunch, QAngle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(GetEyeAngles, QAngle, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(IsScoped, bool, "DT_CSPlayer", "m_bIsScoped");
	NETVAR(GetArmor, int, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(HasDefuser, bool, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(IsDefusing, bool, "DT_CSPlayer", "m_bIsDefusing");
	NETVAR(HasImmunity, bool, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(HasHelmet, bool, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(GetFlashMaxAlpha, float, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(GetFlashDuration, float, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(GetLBY, float, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(GetShotsFired, int, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(GetCompRank, int, "DT_CSPlayerResource", "m_iCompetitiveRanking");
	NETVAR(GetActiveWeapon, CHandle<C_BaseCombatWeapon>, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	PNETVAR(GetWeapons, CHandle<C_BaseCombatWeapon>, "DT_BaseCombatCharacter", "m_hMyWeapons");
	NETVAR(GetWearables, CHandle<C_BaseAttributableItem>, "DT_BaseCombatCharacter", "m_hMyWearables");

	const Vector GetEyePosition()
	{
		return this->GetOrigin() + this->GetViewOffset();
	}

	bool IsAlive()
	{
		return this->GetLifeState() == LIFE_ALIVE;
	}

	Vector GetBonePos(Bone bone)
	{
		matrix3x4_t bone_matrix[128];

		if (this->SetupBones(bone_matrix, 128, 0x7FF00, 0.f))
			return bone_matrix[bone].at(3);

		return Vector{};
	}

	bool IsValid()
	{
		return this->IsPlayer() && !this->IsDormant() && this->IsAlive() && !this->HasImmunity();
	}
};
