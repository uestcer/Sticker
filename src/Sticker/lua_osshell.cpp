#include "stdafx.h"
#include "lua_osshell.h"
#include "osshell.h"

static XLLRTGlobalAPI LuaOSShellFunctions[] = {
	{"GetLocalTime", LuaOSShell::GetLocalTime},
	{"GetCursorPos", LuaOSShell::GetCursorPos},
	{"GetScreenSize", LuaOSShell::GetScreenSize},
	{NULL,NULL}
};

long LuaOSShell::RegisterObject( XL_LRT_ENV_HANDLE hEnv )
{
	if(hEnv == NULL)
	{
		return XLLRT_RESULT_ENV_INVALID;
	}

	XLLRTObject theObject;
	theObject.ClassName = XLUE_LUAOSSHELL_CLASSNAME;
	theObject.MemberFunctions = LuaOSShellFunctions;
	theObject.ObjName = XLUE_LUAOSSHELL_OBJNAME;
	theObject.userData = NULL;
	theObject.pfnGetObject = LuaOSShell::GetObject;

	long lRet = XLLRT_RegisterGlobalObj(hEnv,theObject);
	return lRet;
}

void* LuaOSShell::GetObject( void* ud )
{
	static OSShell osshell;
	return (void*)&osshell;
}

OSShell* LuaOSShell::GetOSShellObjFromLuaState( lua_State* luaState )
{
	OSShell** ppOSShell = (OSShell**)luaL_checkudata(luaState, 1, XLUE_LUAOSSHELL_CLASSNAME);
	if (ppOSShell != NULL)
	{
		return *ppOSShell;
	}
	return NULL;
}

int LuaOSShell::GetLocalTime( lua_State* luaState )
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	lua_newtable(luaState);
	lua_pushstring(luaState, "year");
	lua_pushnumber(luaState, sysTime.wYear);
	lua_settable(luaState, -3);
	lua_pushstring(luaState, "month");
	lua_pushnumber(luaState, sysTime.wMonth);
	lua_settable(luaState, -3);
	lua_pushstring(luaState, "day");
	lua_pushnumber(luaState, sysTime.wDay);
	lua_settable(luaState, -3);
	lua_pushstring(luaState, "dayofweek");
	lua_pushnumber(luaState, sysTime.wDayOfWeek);
	lua_settable(luaState, -3);
	lua_pushstring(luaState, "hour");
	lua_pushnumber(luaState, sysTime.wHour);
	lua_settable(luaState, -3);
	lua_pushstring(luaState, "min");
	lua_pushnumber(luaState, sysTime.wMinute);
	lua_settable(luaState, -3);
	lua_pushstring(luaState, "second");
	lua_pushnumber(luaState, sysTime.wSecond);
	lua_settable(luaState, -3);
	lua_pushstring(luaState, "milliseconds");
	lua_pushnumber(luaState, sysTime.wMilliseconds);
	lua_settable(luaState, -3);

	return 1;
}

int LuaOSShell::GetCursorPos( lua_State* luaState )
{
	POINT point;
	BOOL bRet = ::GetCursorPos(&point);
	if (bRet == TRUE)
	{
		lua_pushnumber(luaState, point.x);
		lua_pushnumber(luaState, point.y);
		return 2;
	}
	else
	{
		return 0;
	}
}

int LuaOSShell::GetScreenSize( lua_State* luaState )
{
	OSShell* pOSShell = GetOSShellObjFromLuaState(luaState);
	SIZE size;
	if (pOSShell->GetScreenSize(size))
	{
		lua_pushnumber(luaState, size.cx);
		lua_pushnumber(luaState, size.cy);
		return 2;
	}
	return 0;
}
