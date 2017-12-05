#pragma once

#define STEAMFRIENDS_INTERFACE_VERSION "SteamFriends015"
#define STEAMHTTP_INTERFACE_VERSION "STEAMHTTP_INTERFACE_VERSION002"
#define STEAMUSER_INTERFACE_VERSION "SteamUser019"

using uint8_t = unsigned char;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;

using HTTPRequestHandle = uint32_t;
enum : HTTPRequestHandle
{
	INVALID_HTTPREQUEST_HANDLE = 0
};

using SteamAPICall_t = uint64_t;
enum : SteamAPICall_t
{
	k_uAPICallInvalid = 0
};

#ifdef DELETE
#undef DELETE
#endif

enum class HTTPMethod
{
	Invalid = 0,
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	OPTIONS,
	PATCH
};

#pragma pack(push, 8)
struct HTTPRequestCompleted_t
{
	enum { k_iCallback = 2101 };
	HTTPRequestHandle m_hRequest;
	uint64_t m_ulContextValue;
	bool m_bRequestSuccessful;
	int m_nStatusCode;
	uint32_t m_unBodySize;
};
#pragma pack(pop)

using HSteamPipe = uint32_t;
using HSteamUser = uint32_t;

class ISteamFriends
{
public:
	virtual const char*        GetPersonaName() = 0;
	virtual SteamAPICall_t     SetPersonaName(const char *pchPersonaName) = 0;
	// more - https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/steam/isteamfriends.h
};

class ISteamHTTP
{
public:
	virtual HTTPRequestHandle  CreateHTTPRequest(HTTPMethod eHTTPRequestMethod, const char* pchAbsoluteURL) = 0;
	virtual bool               SetHTTPRequestContextValue(HTTPRequestHandle hRequest, uint64_t ulContextValue) = 0;
	virtual bool               SetHTTPRequestNetworkActivityTimeout(HTTPRequestHandle hRequest, uint32_t unTimeoutSeconds) = 0;
	virtual bool               SetHTTPRequestHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, const char* pchHeaderValue) = 0;
	virtual bool               SetHTTPRequestGetOrPostParameter(HTTPRequestHandle hRequest, const char* pchParamName, const char* pchParamValue) = 0;
	virtual bool               SendHTTPRequest(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool               SendHTTPRequestAndStreamResponse(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool               DeferHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool               PrioritizeHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool               GetHTTPResponseHeaderSize(HTTPRequestHandle hRequest, const char* pchHeaderName, uint32_t* unResponseHeaderSize) = 0;
	virtual bool               GetHTTPResponseHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, uint8_t* pHeaderValueBuffer, uint32_t unBufferSize) = 0;
	virtual bool               GetHTTPResponseBodySize(HTTPRequestHandle hRequest, uint32_t* unBodySize) = 0;
	virtual bool               GetHTTPResponseBodyData(HTTPRequestHandle hRequest, uint8_t* pBodyDataBuffer, uint32_t unBufferSize) = 0;
	virtual bool               GetHTTPStreamingResponseBodyData(HTTPRequestHandle hRequest, uint32_t cOffset, uint8_t* pBodyDataBuffer, uint32_t unBufferSize) = 0;
	virtual bool               ReleaseHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool               GetHTTPDownloadProgressPct(HTTPRequestHandle hRequest, float* pflPercentOut) = 0;
	virtual bool               SetHTTPRequestRawPostBody(HTTPRequestHandle hRequest, const char* pchContentType, uint8_t* pubBody, uint32_t unBodyLen) = 0;
};

using CGameID = uint32_t;
struct CSteamID
{
	uint32_t account_id;
};

using HAuthTicket = uint32_t;
using AppId_t = uint32_t;

class ISteamUser
{
public:
	virtual HSteamUser                           GetHSteamUser() = 0;
	virtual bool                                 BLoggedOn() = 0;
	virtual CSteamID                             GetSteamID() = 0;
	virtual int                                  InitiateGameConnection(void* pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) = 0;
	virtual void                                 TerminateGameConnection(uint32_t unIPServer, uint16_t usPortServer) = 0;
	virtual void                                 TrackAppUsageEvent(CGameID gameID, int eAppUsageEvent, const char* pchExtraInfo = "") = 0;
	virtual bool                                 GetUserDataFolder(char* pchBuffer, int cubBuffer) = 0;
	virtual void                                 StartVoiceRecording() = 0;
	virtual void                                 StopVoiceRecording() = 0;
	virtual int /*EVoiceResult*/                 GetAvailableVoice(uint32_t* pcbCompressed, uint32_t* pcbUncompressed, uint32_t nUncompressedVoiceDesiredSampleRate) = 0;
	virtual int /*EVoiceResult*/                 GetVoice(bool bWantCompressed, void* pDestBuffer, uint32_t cbDestBufferSize, uint32_t* nBytesWritten, bool bWantUncompressed,
													void* pUncompressedDestBuffer, uint32_t cbUncompressedDestBufferSize, uint32_t* nUncompressBytesWritten,
													uint32_t nUncompressedVoiceDesiredSampleRate) = 0;
	virtual int /*EVoiceResult*/                 DecompressVoice(const void* pCompressed, uint32_t cbCompressed, void* pDestBuffer, uint32_t cbDestBufferSize, uint32_t* nBytesWritten, uint32_t nDesiredSampleRate) = 0;
	virtual uint32_t                             GetVoiceOptimalSampleRate() = 0;
	virtual HAuthTicket                          GetAuthSessionTicket(void* pTicket, int cbMaxTicket, uint32_t* pcbTicket) = 0;
	virtual int /*EBeginAuthSessionResult*/      BeginAuthSession(const void* pAuthTicket, int cbAuthTicket, CSteamID steamID) = 0;
	virtual void                                 EndAuthSession(CSteamID steamID) = 0;
	virtual void                                 CancelAuthTicket(HAuthTicket hAuthTicket) = 0;
	virtual int /*EUserHasLicenseForAppResult*/  UserHasLicenseForApp(CSteamID steamID, AppId_t appID) = 0;
	virtual bool                                 BIsBehindNAT() = 0;
	virtual void                                 AdvertiseGame(CSteamID steamIDGameServer, uint32_t unIPServer, uint16_t usPortServer) = 0;
	virtual SteamAPICall_t                       RequestEncryptedAppTicket(void* pDataToInclude, int cbDataToInclude) = 0;
	virtual bool                                 GetEncryptedAppTicket(void* pTicket, int cbMaxTicket, uint32_t* pcbTicket) = 0;
	virtual int                                  GetGameBadgeLevel(int nSeries, bool bFoil) = 0;
	virtual int                                  GetPlayerSteamLevel() = 0;
};

class ISteamGameServer;
class ISteamUtils;
class ISteamMatchmaking;
class ISteamMatchmakingServers;
class ISteamUserStats;
class ISteamGameServerStats;
class ISteamApps;
class ISteamNetworking;
class ISteamRemoteStorage;
class ISteamScreenshots;
class ISteamUnifiedMessages;
class ISteamController;
class ISteamUGC;
class ISteamAppList;
class ISteamMusic;
class ISteamMusicRemote;
class ISteamHTMLSurface;

class ISteamClient
{
public:
	virtual HSteamPipe                 CreateSteamPipe() = 0; // 0
	virtual bool                       BReleaseSteamPipe(HSteamPipe hSteamPipe) = 0;
	virtual HSteamUser                 ConnectToGlobalUser(HSteamPipe hSteamPipe) = 0;
	virtual HSteamUser                 CreateLocalUser(HSteamPipe* phSteamPipe, int /*EAccountType*/ eAccountType) = 0;
	virtual void                       ReleaseUser(HSteamPipe hSteamPipe, HSteamUser hUser) = 0;
	virtual ISteamUser*                GetISteamUser(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0; // 5
	virtual ISteamGameServer*          GetISteamGameServer(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual void                       SetLocalIPBinding(uint32_t unIP, uint16_t usPort) = 0;
	virtual ISteamFriends*             GetISteamFriends(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0; // 8
	virtual ISteamUtils*               GetISteamUtils(HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMatchmaking*         GetISteamMatchmaking(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0; // 10
	virtual ISteamMatchmakingServers*  GetISteamMatchmakingServers(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual void*                      GetISteamGenericInterface(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUserStats*           GetISteamUserStats(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamGameServerStats*     GetISteamGameServerStats(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamApps*                GetISteamApps(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0; // 15
	virtual ISteamNetworking*          GetISteamNetworking(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamRemoteStorage*       GetISteamRemoteStorage(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamScreenshots*         GetISteamScreenshots(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual void                       RunFrame() = 0;
	virtual uint32_t                   GetIPCCallCount() = 0; // 20
	virtual void                       SetWarningMessageHook(void* /*SteamAPIWarningMessageHook_t*/ pFunction) = 0;
	virtual bool                       BShutdownIfAllPipesClosed() = 0;
	virtual ISteamHTTP*                GetISteamHTTP(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0; // 23
	virtual ISteamUnifiedMessages*     GetISteamUnifiedMessages(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamController*          GetISteamController(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUGC*                 GetISteamUGC(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamAppList*             GetISteamAppList(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMusic*               GetISteamMusic(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMusicRemote*         GetISteamMusicRemote(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamHTMLSurface*         GetISteamHTMLSurface(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	//virtual void                       Set_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	//virtual void                       Remove_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	//virtual void                       Set_SteamAPI_CCheckCallbackRegisteredInProcess(SteamAPI_CheckCallbackRegistered_t func) = 0;
};

extern ISteamFriends* g_steam_friends;
extern ISteamHTTP* g_steam_http;
extern ISteamUser* g_steam_user;
