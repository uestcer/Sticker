#include "stdafx.h"
#include "lua_stickercore.h"
#include "stickercore.h"

lua_State* LuaStickerCore::m_pLuaState = NULL;
UINT LuaStickerCore::m_callbackCookie = 0;
std::vector<int> LuaStickerCore::m_vecCallback;

static XLLRTGlobalAPI LuaStickerCoreFuncs[] = {
	{"ExitSticker", LuaStickerCore::ExitSticker},
	{"AttachListener", LuaStickerCore::AttachListener},
	{"DetachListener", LuaStickerCore::DetachListener},
	{NULL,NULL}
};

long LuaStickerCore::RegisterObject( XL_LRT_ENV_HANDLE hEnv )
{
	if (hEnv == NULL)
	{
		return XLLRT_RESULT_ENV_INVALID;
	}

	XLLRTObject obj;
	obj.ClassName = XLUE_LUASTICKERCORE_CLASSNAME;
	obj.ObjName = XLUE_LUASTICKERCORE_OBJNAME;
	obj.MemberFunctions = LuaStickerCoreFuncs;
	obj.userData = NULL;
	obj.pfnGetObject = LuaStickerCore::GetObject;

	long lRet = XLLRT_RegisterGlobalObj(hEnv, obj);
	return lRet;
}

void* __stdcall LuaStickerCore::GetObject( void* ud )
{
	return NULL;
}

int LuaStickerCore::ExitSticker( lua_State* luaState )
{
	StickerCore::ExitSticker();
	return 0;
}

int LuaStickerCore::AttachListener( lua_State* luaState )
{
	if (lua_isfunction(luaState, 2) != 1)
	{
		return 0;
	}
	lua_settop(luaState, 2);
	if (m_callbackCookie == 0)
	{
		m_callbackCookie = StickerCore::AttachListener(LuaStickerCore::StickerCoreCallback);
		m_pLuaState = luaState;
	}
	int nRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
	m_vecCallback.push_back(nRef);

	lua_pushinteger(luaState, nRef);
	return 1;
}

int LuaStickerCore::DetachListener( lua_State* luaState )
{
	if (lua_isnumber(luaState, 2) != 1)
	{
		return 0;
	}
	int nRef = lua_tointeger(luaState, 2);
	for (std::vector<int>::iterator iter = m_vecCallback.begin(); iter != m_vecCallback.end(); ++iter)
	{
		if (*iter == nRef)
		{
			m_vecCallback.erase(iter);
			luaL_unref(luaState, LUA_REGISTRYINDEX, nRef);
		}
	}
	if (m_vecCallback.empty())
	{
		StickerCore::DetachListener(m_callbackCookie);
		m_callbackCookie = 0;
		m_pLuaState = NULL;
	}

	return 0;
}

void LuaStickerCore::StickerCoreCallback()
{
	if (m_pLuaState == NULL)
	{
		return;
	}
	for (std::vector<int>::iterator iter = m_vecCallback.begin(); iter != m_vecCallback.end(); ++iter)
	{
		int nTop = lua_gettop(m_pLuaState);
		int nRef = *iter;
		lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, nRef);
		lua_pcall(m_pLuaState, 0, 0, 0);
		lua_settop(m_pLuaState, nTop);
	}
}
