#pragma once

#include "UtlVector.hpp"

#define PITCH_NORM      100         // non-pitch shifted
#define PITCH_LOW       95          // other values are possible - 0-255, where 255 is very high
#define PITCH_HIGH      120

#define DEFAULT_SOUND_PACKET_VOLUME 1.0f
#define DEFAULT_SOUND_PACKET_PITCH  100
#define DEFAULT_SOUND_PACKET_DELAY  0.0f

typedef void* FileNameHandle_t;

struct SndInfo_t
{
    int m_nGuid;
    FileNameHandle_t m_filenameHandle;
    int m_nSoundSource;
    int m_nChannel;

    int m_nSpeakerEntity;
    float m_flVolume;
    float m_flLastSpatializedVolume;

    float m_flRadius;
    int m_nPitch;
    Vector *m_pOrigin;
    Vector *m_pDirection;

    bool m_bUpdatePositions;
    bool m_bIsSentence;
    bool m_bDryMix;
    bool m_bSpeaker;
    bool m_bSpecialDSP;
    bool m_bFromServer;
};

enum SoundFlags_t
{
    SND_NOFLAGS = 0,            // to keep the compiler happy
    SND_CHANGE_VOL = (1 << 0),      // change sound vol
    SND_CHANGE_PITCH = (1 << 1),        // change sound pitch
    SND_STOP = (1 << 2),        // stop the sound
    SND_SPAWNING = (1 << 3),        // we're spawning, used in some cases for ambients
                                    // not sent over net, only a param between dll and server.
    SND_DELAY = (1 << 4),       // sound has an initial delay
    SND_STOP_LOOPING = (1 << 5),        // stop all looping sounds on the entity.
    SND_SPEAKER = (1 << 6),     // being played again by a microphone through a speaker

    SND_SHOULDPAUSE = (1 << 7),     // this sound should be paused if the game is paused
    SND_IGNORE_PHONEMES = (1 << 8),
    SND_IGNORE_NAME = (1 << 9),     // used to change all sounds emitted by an entity, regardless of scriptname

    SND_DO_NOT_OVERWRITE_EXISTING_ON_CHANNEL = (1 << 10),
};

enum soundlevel_t
{
    SNDLVL_NONE = 0,

    SNDLVL_20dB = 20,           // rustling leaves
    SNDLVL_25dB = 25,           // whispering
    SNDLVL_30dB = 30,           // library
    SNDLVL_35dB = 35,
    SNDLVL_40dB = 40,
    SNDLVL_45dB = 45,           // refrigerator

    SNDLVL_50dB = 50,   // 3.9  // average home
    SNDLVL_55dB = 55,   // 3.0

    SNDLVL_IDLE = 60,   // 2.0
    SNDLVL_60dB = 60,   // 2.0  // normal conversation, clothes dryer

    SNDLVL_65dB = 65,   // 1.5  // washing machine, dishwasher
    SNDLVL_STATIC = 66, // 1.25

    SNDLVL_70dB = 70,   // 1.0  // car, vacuum cleaner, mixer, electric sewing machine

    SNDLVL_NORM = 75,
    SNDLVL_75dB = 75,   // 0.8  // busy traffic

    SNDLVL_80dB = 80,   // 0.7  // mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
    SNDLVL_TALKING = 80,    // 0.7
    SNDLVL_85dB = 85,   // 0.6  // average factory, electric shaver
    SNDLVL_90dB = 90,   // 0.5  // screaming child, passing motorcycle, convertible ride on frw
    SNDLVL_95dB = 95,
    SNDLVL_100dB = 100, // 0.4  // subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
    SNDLVL_105dB = 105,         // helicopter, power mower
    SNDLVL_110dB = 110,         // snowmobile drvrs seat, inboard motorboat, sandblasting
    SNDLVL_120dB = 120,         // auto horn, propeller aircraft
    SNDLVL_130dB = 130,         // air raid siren

    SNDLVL_GUNFIRE = 140,   // 0.27 // THRESHOLD OF PAIN, gunshot, jet engine
    SNDLVL_140dB = 140, // 0.2

    SNDLVL_150dB = 150, // 0.2

    SNDLVL_180dB = 180,         // rocket launching

                                // NOTE: Valid soundlevel_t values are 0-255.
                                //       256-511 are reserved for sounds using goldsrc compatibility attenuation.
};

class IRecipientFilter;
class Vector;

class IEngineSound
{
public:
    // Precache a particular sample
    virtual bool PrecacheSound(const char *pSample, bool bPreload = false, bool bIsUISound = false) = 0;
    virtual bool IsSoundPrecached(const char *pSample) = 0;
    virtual void PrefetchSound(const char *pSample) = 0;
    virtual bool IsLoopingSound(const char *pSample) = 0;

    // Just loads the file header and checks for duration (not hooked up for .mp3's yet)
    // Is accessible to server and client though
    virtual float GetSoundDuration(const char *pSample) = 0;

    // Pitch of 100 is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
    // down to 1 is a lower pitch.   150 to 70 is the realistic range.
    // EmitSound with pitch != 100 should be used sparingly, as it's not quite as
    // fast (the pitchshift mixer is not native coded).

    // NOTE: setting iEntIndex to -1 will cause the sound to be emitted from the local
    // player (client-side only)
    virtual int EmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
        float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
        const Vector* pOrigin = nullptr, const Vector* pDirection = nullptr, CUtlVector<Vector>* pUtlVecOrigins = nullptr, bool bUpdatePositions = true, float soundtime = 0.f, int speakerentity = -1) = 0;

    virtual int EmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
        float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
        const Vector* pOrigin = nullptr, const Vector* pDirection = nullptr, CUtlVector<Vector>* pUtlVecOrigins = nullptr, bool bUpdatePositions = true, float soundtime = 0.f, int speakerentity = -1) = 0;

    virtual void EmitSentenceByIndex(IRecipientFilter& filter, int iEntIndex, int iChannel, int iSentenceIndex,
        float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
        const Vector *pOrigin = nullptr, const Vector *pDirection = nullptr, CUtlVector<Vector>* pUtlVecOrigins = nullptr, bool bUpdatePositions = true, float soundtime = 0.f, int speakerentity = -1) = 0;

    virtual void    StopSound(int iEntIndex, int iChannel, const char *pSample, unsigned int nSoundEntryHash) = 0;
    virtual void    StopAllSounds(bool bClearBuffers) = 0;
    virtual void    SetRoomType(IRecipientFilter& filter, int roomType) = 0;
    virtual void    SetPlayerDSP(IRecipientFilter& filter, int dspType, bool fastReset) = 0;
    virtual int     EmitAmbientSound(const char *pSample, float flVolume, int iPitch = PITCH_NORM, int flags = 0, float soundtime = 0.0f) = 0;
    virtual float   GetDistGainFromSoundLevel(soundlevel_t soundlevel, float dist) = 0;
    virtual int     GetGuidForLastSoundEmitted() = 0;
    virtual bool    IsSoundStillPlaying(int guid) = 0;
    virtual void    StopSoundByGuid(int guid, bool bForceSync) = 0;
    virtual void    SetVolumeByGuid(int guid, float fvol) = 0;
    virtual void    GetActiveSounds(CUtlVector<SndInfo_t>& sndlist) = 0;
    virtual void    PrecacheSentenceGroup(const char *pGroupName) = 0;
    virtual void    NotifyBeginMoviePlayback() = 0;
    virtual void    NotifyEndMoviePlayback() = 0;
    virtual bool    GetSoundChannelVolume(const char* sound, float &flVolumeLeft, float &flVolumeRight) = 0;
    virtual float   GetElapsedTimeByGuid(int guid) = 0;
};

extern IEngineSound* g_engine_sound;
