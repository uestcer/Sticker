#ifndef _LUA_STICKERCORE_H_
#define _LUA_STICKERCORE_H_

#define XLUE_LUASTICKERCORE_CLASSNAME "Sticker.Core.Class"
#define XLUE_LUASTICKERCORE_OBJNAME "Sticker.Core"

class LuaStickerCore
{
public:
	static long RegisterObject(XL_LRT_ENV_HANDLE hEnv);
private:
	static void* __stdcall GetObject(void* ud);

public:
	static int ExitSticker(lua_State* luaState);
	static int AttachListener(lua_State* luaState);
	static int DetachListener(lua_State* luaState);
private:
	static void StickerCoreCallback();

private:
	static lua_State* m_pLuaState;
	static UINT m_callbackCookie;
	static std::vector<int> m_vecCallback;
};

#endif