#pragma once 

#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientThinkable.hpp"
#include "IClientUnknown.hpp"

#include "CHandle.hpp"
#include "VMatrix.hpp"

#include "../utils.hpp"

enum LifeState
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
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
	NETVAR(GetTeam, int, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(GetOwnerEntity, CHandle<C_BaseEntity>, "DT_BaseEntity", "m_hOwnerEntity");

	void SetModelIndex(int index)
	{
		using oSetModelIndex = void(__thiscall*)(void*, int);
		return get_vfunc<oSetModelIndex>(this, 75)(this, index);
	}
};

class C_BaseViewModel;
class C_BaseCombatWeapon;
class C_BaseAttributableItem;

class C_BasePlayer : public C_BaseEntity
{
public:
	NETVAR(GetFlags, int, "DT_BasePlayer", "m_fFlags");
	NETVAR(GetHealth, int, "DT_BasePlayer", "m_iHealth");
	NETVAR(GetLifeState, LifeState, "DT_BasePlayer", "m_lifeState");
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

	const Vector& GetEyePosition()
	{
		return this->GetVecOrigin() + this->GetVecViewOffset();
	}

	bool IsAlive()
	{
		return this->GetLifeState() == LIFE_ALIVE && this->GetHealth() > 0;
	}

	Vector GetBonePos(int bone)
	{
		matrix3x4_t bone_matrix[128];

		if (this->SetupBones(bone_matrix, 128, 0x7FF00, 0.f))
			return bone_matrix[bone].at(3);

		return Vector{};
	}

	
};
