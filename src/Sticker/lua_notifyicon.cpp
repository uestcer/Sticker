#include "lua_notifyicon.h"
#include "notifyicon.h"
#include "transcode.h"


static XLLRTGlobalAPI LuaNotifyIconFunctions[] = {
	{"AddIcon", LuaNotifyIcon::AddIcon},
	{"DelIcon", LuaNotifyIcon::DelIcon},
	{"ModIcon", LuaNotifyIcon::ModIcon},
	{"Attach", LuaNotifyIcon::Attach},
	{"Detach", LuaNotifyIcon::Detach},
	{NULL,NULL}
};

long LuaNotifyIcon::RegisterObject( XL_LRT_ENV_HANDLE hEnv )
{
	if(hEnv == NULL)
	{
		return XLLRT_RESULT_ENV_INVALID;
	}

	XLLRTObject theObject;
	theObject.ClassName = XLUE_LUANOTIFYICON_CLASSNAME;
	theObject.MemberFunctions = LuaNotifyIconFunctions;
	theObject.ObjName = XLUE_LUANOTIFYICON_OBJNAME;
	theObject.userData = NULL;
	theObject.pfnGetObject = LuaNotifyIcon::GetObject;

	long lRet = XLLRT_RegisterGlobalObj(hEnv,theObject);
	return lRet;
}

void* __stdcall LuaNotifyIcon::GetObject( void* ud )
{
	return NULL;
}

int LuaNotifyIcon::AddIcon( lua_State* luaState )
{
	std::wstring wstrIconPath;
	const char* strIconPath = luaL_checkstring(luaState, 2);
	TransCode::UTF8_to_Unicode(strIconPath, strlen(strIconPath), wstrIconPath);
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HICON hIcon = LoadIcon(hInstance, wstrIconPath.c_str());
	if (hIcon == NULL)
	{
		return 0;
	}

	WCHAR* wstrTip = NULL;
	if (lua_isstring(luaState, 3) == 1)
	{
		const char* strTip = lua_tostring(luaState, 3);
		std::wstring wstr;
		TransCode::UTF8_to_Unicode(strTip, strlen(strTip), wstr);
		wstrTip = wstr.c_str();
	}

	UINT uID = NotifyIcon::AddIcon(hIcon, wstrTip);
	if (uID > 0)
	{
		lua_pushinteger(luaState, uID);
		return 1;
	}
	return 0;
}

int LuaNotifyIcon::DelIcon( lua_State* luaState )
{
	UINT uID = luaL_checkinteger(luaState, 2);
	NotifyIcon::DelIcon(uID);

	return 0;
}

int LuaNotifyIcon::ModIcon( lua_State* luaState )
{
	UINT uID = luaL_checkinteger(luaState, 2);
	HICON hIcon = NULL;
	WCHAR* wstrTip = NULL;
	if (lua_isstring(luaState, 3) == 1)
	{
		std::wstring wstrIconPath;
		const char* strIconPath = lua_tostring(luaState, 3);
		TransCode::UTF8_to_Unicode(strIconPath, strlen(strIconPath), wstrIconPath);
		HINSTANCE hInstance = GetModuleHandle(NULL);
		hIcon = LoadIcon(hInstance, wstrIconPath.c_str());
	}
	if (lua_isstring(luaState, 4) == 1)
	{
		const char* strTip = lua_tostring(luaState, 4);
		std::wstring wstr;
		TransCode::UTF8_to_Unicode(strTip, strlen(strTip), wstr);
		wstrTip = wstr.c_str();
	}

	NotifyIcon::ModIcon(uID, hIcon, wstrTip);
	
	return 0;
}

int LuaNotifyIcon::Attach( lua_State* luaState )
{
	return 0;
}

int LuaNotifyIcon::Detach( lua_State* luaState )
{
	return 0;
}
