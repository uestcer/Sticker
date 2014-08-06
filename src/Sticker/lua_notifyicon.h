#ifndef _LUA_NOTIFYICON_H_
#define _LUA_NOTIFYICON_H_

#include <windows.h>
#include <XLUE.h>
#include <XLGraphic.h>
#include <XLLuaRuntime.h>

#define XLUE_LUANOTIFYICON_CLASSNAME "Sticker.Helper.NotifyIcon.Class"
#define XLUE_LUANOTIFYICON_OBJNAME "Sticker.Helper.NotifyIcon"

class LuaNotifyIcon
{
public:
	static long RegisterObject(XL_LRT_ENV_HANDLE hEnv);
private:
	static void* __stdcall GetObject(void* ud);

public:
	static int AddIcon(lua_State* luaState);
	static int DelIcon(lua_State* luaState);
	static int ModIcon(lua_State* luaState);
	static int Attach(lua_State* luaState);
	static int Detach(lua_State* luaState);
};

#endif