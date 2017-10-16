#pragma once

#include "VMatrix.hpp"
#include "Vector4D.hpp"

#pragma region MASKS

#define   DISPSURF_FLAG_SURFACE         (1 << 0)
#define   DISPSURF_FLAG_WALKABLE        (1 << 1)
#define   DISPSURF_FLAG_BUILDABLE       (1 << 2)
#define   DISPSURF_FLAG_SURFPROP1       (1 << 3)
#define   DISPSURF_FLAG_SURFPROP2       (1 << 4)

#define   CONTENTS_EMPTY                0
#define   CONTENTS_SOLID                (1 << 0)
#define   CONTENTS_WINDOW               (1 << 1)
#define   CONTENTS_AUX                  (1 << 2)
#define   CONTENTS_GRATE                (1 << 3)
#define   CONTENTS_SLIME                (1 << 4)
#define   CONTENTS_WATER                (1 << 5)
#define   CONTENTS_BLOCKLOS             (1 << 6)
#define   CONTENTS_OPAQUE               (1 << 7)
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE
#define   ALL_VISIBLE_CONTENTS          (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS - 1))
#define   CONTENTS_TESTFOGVOLUME        (1 << 8)
#define   CONTENTS_UNUSED               (1 << 9)
#define   CONTENTS_BLOCKLIGHT           (1 << 10)
#define   CONTENTS_TEAM1                (1 << 11)
#define   CONTENTS_TEAM2                (1 << 12)
#define   CONTENTS_IGNORE_NODRAW_OPAQUE (1 << 13)
#define   CONTENTS_MOVEABLE             (1 << 14)
#define   CONTENTS_AREAPORTAL           (1 << 15)
#define   CONTENTS_PLAYERCLIP           (1 << 16)
#define   CONTENTS_MONSTERCLIP          (1 << 17)
#define   CONTENTS_CURRENT_0            (1 << 18)
#define   CONTENTS_CURRENT_90           (1 << 19)
#define   CONTENTS_CURRENT_180          (1 << 20)
#define   CONTENTS_CURRENT_270          (1 << 21)
#define   CONTENTS_CURRENT_UP           (1 << 22)
#define   CONTENTS_CURRENT_DOWN         (1 << 23)
#define   CONTENTS_ORIGIN               (1 << 24)
#define   CONTENTS_MONSTER              (1 << 25)
#define   CONTENTS_DEBRIS               (1 << 26)
#define   CONTENTS_DETAIL               (1 << 27)
#define   CONTENTS_TRANSLUCENT          (1 << 28)
#define   CONTENTS_LADDER               (1 << 29)
#define   CONTENTS_HITBOX               (1 << 30)

#define   SURF_LIGHT                    (1 << 0)
#define   SURF_SKY2D                    (1 << 1)
#define   SURF_SKY                      (1 << 2)
#define   SURF_WARP                     (1 << 3)
#define   SURF_TRANS                    (1 << 4)
#define   SURF_NOPORTAL                 (1 << 5)
#define   SURF_TRIGGER                  (1 << 6)
#define   SURF_NODRAW                   (1 << 7)
#define   SURF_HINT                     (1 << 8)
#define   SURF_SKIP                     (1 << 9)
#define   SURF_NOLIGHT                  (1 << 10)
#define   SURF_BUMPLIGHT                (1 << 11)
#define   SURF_NOSHADOWS                (1 << 12)
#define   SURF_NODECALS                 (1 << 13)
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   (1 << 14)
#define   SURF_HITBOX                   (1 << 15)

#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE | CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS | CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER | CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW|CONTENTS_GRATE)

#pragma endregion

class IHandleEntity;
class IClientEntity;
struct Ray_t;
class CGameTrace;
using trace_t = CGameTrace;
class ICollideable;
class ITraceListData;
class CPhysCollide;
struct virtualmeshlist_t;
struct cplane_t;

enum TraceType
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntity, int contentsMask) = 0;
	virtual TraceType GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}
	void* pSkip;
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_ENTITIES_ONLY;
	}
};

class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		return false;
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_WORLD_ONLY;
	}
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		return false;
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}
};

class CTraceFilterHitAll : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		return true;
	}
};

enum DebugTraceCounterBehavior_t
{
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};

class IEntityEnumerator
{
public:
	virtual bool EnumEntity(IHandleEntity* pHandleEntity) = 0;
};

struct BrushSideInfo_t
{
	Vector4D plane;          // The plane of the brush side
	unsigned short bevel;    // Bevel plane?
	unsigned short thin;     // Thin?
};

class CPhysCollide;

struct vcollide_t
{
	unsigned short   solidCount : 15;
	unsigned short   isPacked : 1;
	unsigned short   descSize;
	// VPhysicsSolids
	CPhysCollide**   solids;
	char*            pKeyValues;
	void*            pUserData;
};

struct cmodel_t
{
	Vector           mins, maxs;
	Vector           origin; // for sounds & lights
	int              headnode;
	vcollide_t       vcollisionData;
};

struct csurface_t
{
	const char*      name;
	short            surfaceProps;
	unsigned short   flags; // BUG: These are declared per surface, not per material, but this database is per-material now
};

struct Ray_t
{
	VectorAligned           m_Start;                 // starting point, centered within the extents
	VectorAligned           m_Delta;                 // direction + length of the ray
	VectorAligned           m_StartOffset;           // Add this to m_Start to get the actual ray start
	VectorAligned           m_Extents;               // Describes an axis aligned box extruded along a ray
	const matrix3x4_t*      m_pWorldAxisTransform;
	bool                    m_IsRay;                 // are the extents zero?
	bool                    m_IsSwept;               // is delta != 0?

	Ray_t() : m_pWorldAxisTransform(NULL) {}

	void Init(Vector const &start, Vector const &end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();

		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// offset m_Start to be in the center of the box...
		m_StartOffset.Init();
		VectorCopy(start, m_Start);
	}

	void Init(Vector const &start, Vector const &end, Vector const &mins, Vector const &maxs)
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// pffset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis) {
			if (m_Delta[iAxis] != 0.0f) {
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else {
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}
};

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	// these members are aligned!!
	Vector           startpos;       // start position
	Vector           endpos;         // final position
	cplane_t         plane;          // surface normal at impact

	float            fraction;       // time completed, 1.0 = didn't hit anything

	int              contents;       // contents on other side of surface hit
	unsigned short   dispFlags;      // displacement flags for marking surfaces with data

	bool             allsolid;       // if true, plane is not valid
	bool             startsolid;     // if true, the initial point was in a solid area

	CBaseTrace() {}
};

class CGameTrace : public CBaseTrace
{
public:
	bool   DidHitWorld() const;
	bool   DidHitNonWorldEntity() const;
	int    GetEntityIndex() const;
	bool   DidHit() const;
	bool   IsVisible() const;

public:
	float               fractionleftsolid;    // time we left a solid, only valid if we started in solid
	csurface_t          surface;              // surface hit (impact surface)
	int                 hitgroup;             // 0 == generic, non-zero is specific body part
	short               physicsbone;          // physics bone hit by trace in studio
	unsigned short      worldSurfaceIndex;    // Index of the msurface2_t, if applicable
	IClientEntity*      m_pEnt;
	int                 hitbox;               // box hit by trace in studio

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& other) :
		fractionleftsolid(other.fractionleftsolid),
		surface(other.surface),
		hitgroup(other.hitgroup),
		physicsbone(other.physicsbone),
		worldSurfaceIndex(other.worldSurfaceIndex),
		m_pEnt(other.m_pEnt),
		hitbox(other.hitbox)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
	}

	CGameTrace& CGameTrace::operator=(const CGameTrace& other)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
		fractionleftsolid = other.fractionleftsolid;
		surface = other.surface;
		hitgroup = other.hitgroup;
		physicsbone = other.physicsbone;
		worldSurfaceIndex = other.worldSurfaceIndex;
		m_pEnt = other.m_pEnt;
		hitbox = other.hitbox;
		return *this;
	}
};

inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}

inline bool CGameTrace::IsVisible() const
{
	return fraction > 0.97f;
}

class IEngineTrace
{
public:
	virtual int         GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = nullptr) = 0;
	virtual int         GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = MASK_ALL) = 0;
	virtual int         GetPointContents_Collideable(ICollideable* pCollide, const Vector &vecAbsPosition) = 0;
	virtual void        ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity* pEnt, CGameTrace* pTrace) = 0;
	virtual void        ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, ICollideable* pCollide, CGameTrace* pTrace) = 0;
	virtual void        TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace) = 0;
};
