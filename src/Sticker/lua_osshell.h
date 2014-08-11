#ifndef _LUA_OSSHELL_H_
#define _LUA_OSSHELL_H_

#define XLUE_LUAOSSHELL_CLASSNAME "Sticker.Helper.OSShell.Class"
#define XLUE_LUAOSSHELL_OBJNAME "Sticker.Helper.OSShell"

class LuaOSShell
{
public:
	static long RegisterObject(XL_LRT_ENV_HANDLE hEnv);
private:
	static void* __stdcall GetObject(void* ud);

public:
	static int GetLocalTime(lua_State* luaState);
	static int GetCursorPos(lua_State* luaState);
};

#endif