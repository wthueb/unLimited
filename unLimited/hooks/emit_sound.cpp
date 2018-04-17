#include "hooks.hpp"

#include "../vmt_hook.hpp"

void __fastcall hooks::hk_emit_sound(void* thisptr, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
	float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch,
	const Vector* pOrigin, const Vector* pDirection, CUtlVector<Vector>* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity)
{
	static const auto o_emit_sound = engine_sound_hook->get_original<void(__thiscall*)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, float, int, int, int,
		const Vector*, const Vector*, CUtlVector<Vector>*, bool, float, int)>(index::emit_sound);

	if (strstr(pSample, "null"))
	{
		iFlags = SND_STOP;
	}

	o_emit_sound(thisptr, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
}
