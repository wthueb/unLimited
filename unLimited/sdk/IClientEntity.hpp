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

enum ItemDefinitionIndex_t
{
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_ump = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516,
	studded_bloodhound_gloves = 5027,
	t_gloves = 5028,
	ct_gloves = 5029,
	sporty_gloves = 5030,
	slick_gloves = 5031,
	leather_handwraps = 5032,
	motorcycle_gloves = 5033,
	specialist_gloves = 5034
};

enum Bone_t
{
	INVALID = -1,
	BONE_PELVIS,
	LEAN_ROOT,
	CAM_DRIVER,
	BONE_HIP,
	BONE_LOWER_SPINAL_COLUMN,
	BONE_MIDDLE_SPINAL_COLUMN,
	BONE_UPPER_SPINAL_COLUMN,
	BONE_NECK,
	BONE_HEAD,
};

enum LifeState_t
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

enum Team_t
{
	TEAM_UNASSIGNED = 0,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST,
};

enum Flags_t
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

enum MoveType_t
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

enum ObserverMode_t
{
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM,
	OBS_MODE_FREEZECAM,
	OBS_MODE_FIXED,
	OBS_MODE_IN_EYE,
	OBS_MODE_CHASE,
	OBS_MODE_ROAMING
};

enum CSWeaponType_t
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

enum WeaponSound_t
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
	NETVAR(GetVecOrigin, Vector, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(GetSpotted, bool, "DT_BaseEntity", "m_bSpotted");
	NETVAR(GetModelIndex, int, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(GetTeam, Team_t, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(GetOwnerEntity, CHandle<C_BaseEntity>, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(GetShouldGlow, bool, "DT_DynamicProp", "m_bShouldGlow");

	void SetModelIndex(int index)
	{
		using oSetModelIndex = void(__thiscall*)(void*, int);
		return get_vfunc<oSetModelIndex>(this, 75)(this, index);
	}

	bool IsWeapon()
	{
		return get_vfunc<bool(__thiscall*)(C_BaseEntity*)>(this, 160)(this);
	}
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(GetItemDefinitionIndex, ItemDefinitionIndex_t, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(GetFallbackPaintKit, int, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(GetFallbackSeed, int, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(GetFallbackWear, int, "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(GetFallbackStatTrak, int, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(GetItemIDHigh, int, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(GetAccountID, int, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(GetEntityQuality, int, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(GetCustomName, char*, "DT_BaseAttributableItem", "m_szCustomName");
};

class C_BaseViewModel : public C_BaseEntity
{
public:
	NETVAR(GetWeapon, int, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(GetOwner, int, "DT_BaseViewModel", "m_hOwner");
};

class CHudTexture;
class KeyValues;

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

		return weaponid == weapon_deagle || weaponid == weapon_elite ||
			weaponid == weapon_fiveseven || weaponid == weapon_p250 ||
			weaponid == weapon_glock || weaponid == weapon_hkp2000 ||
			weaponid == weapon_cz75a || weaponid == weapon_usp_silencer ||
			weaponid == weapon_tec9 || weaponid == weapon_revolver;
	}

	bool IsShotgun()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == weapon_xm1014 || weaponid == weapon_nova ||
			weaponid == weapon_sawedoff || weaponid == weapon_mag7;
	}

	bool IsSniper()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == weapon_awp || weaponid == weapon_scar20 ||
			weaponid == weapon_g3sg1 || weaponid == weapon_ssg08;
	}

	bool IsNade()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == weapon_flashbang || weaponid == weapon_hegrenade ||
			weaponid == weapon_smokegrenade || weaponid == weapon_molotov ||
			weaponid == weapon_decoy || weaponid == weapon_incgrenade;
	}

	bool IsKnife()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == weapon_knife || weaponid == weapon_knife_t ||
			weaponid >= 500;
	}

	bool IsBomb()
	{
		int weaponid = GetItemDefinitionIndex();

		return weaponid == weapon_c4;
	}
};

class C_BasePlayer : public C_BaseEntity
{
public:
	NETVAR(GetFlags, Flags_t, "DT_BasePlayer", "m_fFlags");
	NETVAR(GetHealth, int, "DT_BasePlayer", "m_iHealth");
	NETVAR(GetLifeState, LifeState_t, "DT_BasePlayer", "m_lifeState");
	NETVAR(GetObserverTarget, CHandle<C_BasePlayer>, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(GetViewModel, CHandle<C_BaseViewModel>, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(GetVecViewOffset, Vector, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(GetTickBase, int, "DT_BasePlayer", "m_nTickBase");
	NETVAR(GetVecVelocity, Vector, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(GetViewPunch, QAngle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(GetAimPunch, QAngle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(GetEyeAngles, QAngle, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(GetFlashMaxAlpha, float, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(IsScoped, bool, "DT_CSPlayer", "m_bIsScoped");
	NETVAR(GetArmor, int, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(HasDefuser, bool, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(IsDefusing, bool, "DT_CSPlayer", "m_bIsDefusing");
	NETVAR(HasImmunity, bool, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(HasHelmet, bool, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(GetFlashDuration, float, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(GetLBY, float, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(GetShotsFired, int, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(GetCompRank, int, "DT_CSPlayerResource", "m_iCompetitiveRanking");
	NETVAR(GetActiveWeapon, CHandle<C_BaseCombatWeapon>, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	PNETVAR(GetWeapons, CHandle<C_BaseCombatWeapon>, "DT_BaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(GetWearables, CHandle<C_BaseAttributableItem>, "DT_BaseCombatCharacter", "m_hMyWearables");

	const Vector GetEyePosition()
	{
		return this->GetVecOrigin() + this->GetVecViewOffset();
	}

	bool IsAlive()
	{
		return this->GetLifeState() == LIFE_ALIVE;
	}

	Vector GetBonePos(Bone_t bone)
	{
		matrix3x4_t bone_matrix[128];

		if (this->SetupBones(bone_matrix, 128, 0x7FF00, 0.f))
			return bone_matrix[bone].at(3);

		return Vector{};
	}

	bool IsValid()
	{
		return !IsDormant() && IsAlive() && !HasImmunity();
	}
};
