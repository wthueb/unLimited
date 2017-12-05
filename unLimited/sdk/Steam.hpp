#pragma once

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

struct CSteamID
{
	uint32_t m_steamid;
};

class ISteamFriends
{
public:
	virtual const char* GetPersonaName() = 0;
	virtual SteamAPICall_t SetPersonaName(const char *pchPersonaName) = 0;
	// more
};

extern ISteamFriends* g_steam_friends;

class ISteamHTTP
{
public:
	virtual HTTPRequestHandle CreateHTTPRequest(HTTPMethod eHTTPRequestMethod, const char* pchAbsoluteURL) = 0;
	virtual bool SetHTTPRequestContextValue(HTTPRequestHandle hRequest, uint64_t ulContextValue) = 0;
	virtual bool SetHTTPRequestNetworkActivityTimeout(HTTPRequestHandle hRequest, uint32_t unTimeoutSeconds) = 0;
	virtual bool SetHTTPRequestHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, const char* pchHeaderValue) = 0;
	virtual bool SetHTTPRequestGetOrPostParameter(HTTPRequestHandle hRequest, const char* pchParamName, const char* pchParamValue) = 0;
	virtual bool SendHTTPRequest(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool SendHTTPRequestAndStreamResponse(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool DeferHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool PrioritizeHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool GetHTTPResponseHeaderSize(HTTPRequestHandle hRequest, const char* pchHeaderName, uint32_t* unResponseHeaderSize) = 0;
	virtual bool GetHTTPResponseHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, uint8_t* pHeaderValueBuffer, uint32_t unBufferSize) = 0;
	virtual bool GetHTTPResponseBodySize(HTTPRequestHandle hRequest, uint32_t* unBodySize) = 0;
	virtual bool GetHTTPResponseBodyData(HTTPRequestHandle hRequest, uint8_t* pBodyDataBuffer, uint32_t unBufferSize) = 0;
	virtual bool GetHTTPStreamingResponseBodyData(HTTPRequestHandle hRequest, uint32_t cOffset, uint8_t* pBodyDataBuffer, uint32_t unBufferSize) = 0;
	virtual bool ReleaseHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool GetHTTPDownloadProgressPct(HTTPRequestHandle hRequest, float* pflPercentOut) = 0;
	virtual bool SetHTTPRequestRawPostBody(HTTPRequestHandle hRequest, const char* pchContentType, uint8_t* pubBody, uint32_t unBodyLen) = 0;
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

extern ISteamHTTP* g_steam_http;
