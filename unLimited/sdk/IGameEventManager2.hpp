#pragma once

#include <cstdint>
#include <functional>

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class bf_read;
class bf_write;

class IGameEvent;
class KeyValues;

class IGameEvent
{
public:
	virtual					~IGameEvent() {};
	virtual const char*		GetName() const = 0;

	virtual bool			IsReliable() const = 0;
	virtual bool			IsLocal() const = 0;
	virtual bool			IsEmpty(const char* keyname = nullptr) = 0;

	virtual bool			GetBool(const char* keyname = nullptr, bool default_value = false) = 0;
	virtual int				GetInt(const char* keyname = nullptr, int default_value = 0) = 0;
	virtual uint64_t		GetUint64(const char* keyname = nullptr, uint64_t default_value = 0) = 0;
	virtual float			GetFloat(const char* keyname = nullptr, float default_value = 0.0f) = 0;
	virtual const char*		GetString(const char* keyname = nullptr, const char* default_value = "") = 0;
	virtual const wchar_t*  GetWString(const char* keyname = nullptr, const wchar_t* default_value = L"") = 0;
	virtual const void*		GetPtr(const char* keyname = nullptr, const void* default_values = nullptr) = 0;

	virtual void			SetBool(const char* keyname, bool value) = 0;
	virtual void			SetInt(const char* keyname, int value) = 0;
	virtual void			SetUint64(const char* keyname, uint64_t value) = 0;
	virtual void			SetFloat(const char* keyname, float value) = 0;
	virtual void			SetString(const char* keyname, const char* value) = 0;
	virtual void			SetWString(const char* keyname, const wchar_t* value) = 0;
	virtual void			SetPtr(const char* keyname, const void* value) = 0;
};

class IGameEventListener2
{
public:
	virtual       ~IGameEventListener2() {};

	virtual void  FireGameEvent(IGameEvent *event) = 0;
	virtual int   GetEventDebugID() = 0;
};

#ifdef CreateEvent
#undef CreateEvent
#endif

class IGameEventManager2
{
public:
	virtual              ~IGameEventManager2() {}; // 0
	virtual int          LoadEventsFromFile(const char* filename) = 0; // 1
	virtual void         Reset() = 0; // 2
	virtual bool         AddListener(IGameEventListener2* listener, const char* name, bool serverside = false) = 0; // 3
	virtual bool         FindListener(IGameEventListener2* listener, const char* name) = 0; // 4
	virtual void         RemoveListener(IGameEventListener2* listener) = 0; // 5
	virtual void         AddListenerGlobal(IGameEventListener2* listener, bool serverside) = 0; // 6
	virtual IGameEvent*  CreateEvent(const char* name, bool force = false, int* cookie = nullptr) = 0; // 7
	virtual bool         FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0; // 8
	virtual bool         FireEventClientSide(IGameEvent* event) = 0; // 9
	virtual IGameEvent*  DuplicateEvent(IGameEvent* event) = 0; // 10
	virtual void         FreeEvent(IGameEvent* event) = 0; // 11
	virtual bool         SerializeEvent(IGameEvent* event, bf_write* buffer) = 0; // 12
	virtual IGameEvent*  UnserializeEvent(bf_read* buffer) = 0; // 13
	virtual KeyValues*   GetEventDataTypes(IGameEvent* event) = 0; // 14
};

extern IGameEventManager2* g_game_event_manager;

class EventListener : public IGameEventListener2
{
public:
	EventListener(const std::string& name, std::function<void(IGameEvent*)> func)
		: m_event_name{ name }, m_func{ func }
	{
		g_game_event_manager->AddListener(this, m_event_name.c_str(), false);
	}

	void shutdown()
	{
		g_game_event_manager->RemoveListener(this);
	}

	void FireGameEvent(IGameEvent* event) override
	{
		m_func(event);
	}

	int GetEventDebugID() override
	{
		return 42;
	}

private:
	std::string m_event_name;
	std::function<void(IGameEvent*)> m_func;
};
