#ifndef _LUA_NOTIFYICON_H_
#define _LUA_NOTIFYICON_H_

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
private:
	static VOID NotifyIconCallbackFunc(UINT uID, UINT message);

private:
	static lua_State* m_pLuaState;
	static UINT m_callbackCookie;
	static std::vector<int> m_vecCallback;    // LuaNotifyIcon:Attach 时回调函数的 ref
};

#endif