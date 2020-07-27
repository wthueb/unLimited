#pragma once

#include "QAngle.hpp"
#include "Vector.hpp"

#define STUDIO_NONE                     0x00000000
#define STUDIO_RENDER                   0x00000001
#define STUDIO_VIEWXFORMATTACHMENTS     0x00000002
#define STUDIO_DRAWTRANSLUCENTSUBMODELS 0x00000004
#define STUDIO_TWOPASS                  0x00000008
#define STUDIO_STATIC_LIGHTING          0x00000010
#define STUDIO_WIREFRAME                0x00000020
#define STUDIO_ITEM_BLINK               0x00000040
#define STUDIO_NOSHADOWS                0x00000080
#define STUDIO_WIREFRAME_VCOLLIDE       0x00000100

// Not a studio flag, but used to flag when we want studio stats
#define STUDIO_GENERATE_STATS           0x01000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SSAODEPTHTEXTURE         0x08000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SHADOWDEPTHTEXTURE       0x40000000

// Not a studio flag, but used to flag model as a non-sorting brush model
#define STUDIO_TRANSPARENCY             0x80000000

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8_t;

class matrix3x4_t;
class IClientUnknown;
struct model_t;

class IClientRenderable
{
public:
    virtual IClientUnknown*            GetIClientUnknown() = 0;
    virtual Vector const&              GetRenderOrigin(void) = 0;
    virtual QAngle const&              GetRenderAngles(void) = 0;
    virtual bool                       ShouldDraw(void) = 0;
    virtual int                        GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
    virtual void                       Unused(void) const {}
    virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
    virtual ClientRenderHandle_t&      RenderHandle() = 0;
    virtual const model_t*             GetModel() const = 0;
    virtual int                        DrawModel(int flags, const int& /*RenderableInstance_t*/ instance) = 0; // 9
    virtual int                        GetBody() = 0;
    virtual void                       GetColorModulation(float* color) = 0;
    virtual bool                       LODTest() = 0;
    virtual bool                       SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
    virtual void                       SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights) = 0;
    virtual void                       DoAnimationEvents(void) = 0;
    virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
    virtual void                       GetRenderBounds(Vector& mins, Vector& maxs) = 0;
    virtual void                       GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
    virtual void                       GetShadowRenderBounds(Vector &mins, Vector &maxs, int /*ShadowType_t*/ shadowType) = 0;
    virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
    virtual bool                       GetShadowCastDistance(float *pDist, int /*ShadowType_t*/ shadowType) const = 0;
    virtual bool                       GetShadowCastDirection(Vector *pDirection, int /*ShadowType_t*/ shadowType) const = 0;
    virtual bool                       IsShadowDirty() = 0;
    virtual void                       MarkShadowDirty(bool bDirty) = 0;
    virtual IClientRenderable*         GetShadowParent() = 0;
    virtual IClientRenderable*         FirstShadowChild() = 0;
    virtual IClientRenderable*         NextShadowPeer() = 0;
    virtual int /*ShadowType_t*/       ShadowCastType() = 0;
    virtual void                       CreateModelInstance() = 0;
    virtual ModelInstanceHandle_t      GetModelInstance() = 0;
    virtual const matrix3x4_t&         RenderableToWorldTransform() = 0;
    virtual int                        LookupAttachment(const char *pAttachmentName) = 0;
    virtual   bool                     GetAttachment(int number, Vector &origin, QAngle &angles) = 0;
    virtual bool                       GetAttachment(int number, matrix3x4_t &matrix) = 0;
    virtual float*                     GetRenderClipPlane(void) = 0;
    virtual int                        GetSkin() = 0;
    virtual void                       OnThreadedDrawSetup() = 0;
    virtual bool                       UsesFlexDelayedWeights() = 0;
    virtual void                       RecordToolMessage() = 0;
    virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
    virtual uint8_t                      OverrideAlphaModulation(uint8_t nAlpha) = 0;
    virtual uint8_t                      OverrideShadowAlphaModulation(uint8_t nAlpha) = 0;
};
