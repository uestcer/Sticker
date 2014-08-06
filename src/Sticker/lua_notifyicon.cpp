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

std::vector<int> LuaNotifyIcon::m_vecCallback;
UINT LuaNotifyIcon::m_callbackCookie = 0;
lua_State* LuaNotifyIcon::m_pLuaState = NULL;

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
	HICON hIcon = (HICON)LoadImage(NULL, wstrIconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	if (hIcon == NULL)
	{
		return 0;
	}

	std::wstring wstrTip;
	if (lua_isstring(luaState, 3) == 1)
	{
		const char* strTip = lua_tostring(luaState, 3);
		TransCode::UTF8_to_Unicode(strTip, strlen(strTip), wstrTip);
	}

	UINT uID;
	if (wstrTip.empty())
	{
		uID = NotifyIcon::AddIcon(hIcon, NULL);
	}
	else
	{
		uID = NotifyIcon::AddIcon(hIcon, wstrTip.c_str());
	}
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
	std::wstring wstrTip;
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
		TransCode::UTF8_to_Unicode(strTip, strlen(strTip), wstrTip);
	}

	if (wstrTip.empty())
	{
		NotifyIcon::ModIcon(uID, hIcon, NULL);
	}
	else
	{
		NotifyIcon::ModIcon(uID, hIcon, wstrTip.c_str());
	}
	
	return 0;
}

int LuaNotifyIcon::Attach( lua_State* luaState )
{
	if (lua_isfunction(luaState, 2) != 1)
	{
		return 0;
	}
	if (m_callbackCookie == 0)
	{
		// 监听来自 NotifyIcon 的事件
		m_callbackCookie = NotifyIcon::Attach(LuaNotifyIcon::NotifyIconCallbackFunc);
		m_pLuaState = luaState;
	}
	if (m_callbackCookie == 0)
	{
		return 0;
	}
	lua_settop(luaState, 2);
	int nRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
	m_vecCallback.push_back(nRef);

	lua_pushinteger(luaState, nRef);
	return 1;
}

int LuaNotifyIcon::Detach( lua_State* luaState )
{
	if (lua_isnumber(luaState, 2) != 1)
	{
		return 0;
	}
	int nRef = luaL_checkinteger(luaState, 2);
	for (std::vector<int>::iterator iter = m_vecCallback.begin(); iter != m_vecCallback.end(); ++iter)
	{
		if (*iter == nRef)
		{
			m_vecCallback.erase(iter);
			luaL_unref(luaState, LUA_REGISTRYINDEX, nRef);
		}
	}
	if (m_vecCallback.empty() && m_callbackCookie != 0)
	{
		// 没有回调函数了，取消对 NotifyIcon 时间的监听
		NotifyIcon::Detach(m_callbackCookie);
		m_callbackCookie = 0;
		m_pLuaState = NULL;
	}

	return 0;
}

VOID LuaNotifyIcon::NotifyIconCallbackFunc( UINT uID, UINT message )
{
	if (m_pLuaState == NULL)
	{
		return;
	}
	std::wstring wstrMessage;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		wstrMessage = L"WM_LBUTTONDOWN";
		break;
	case WM_LBUTTONUP:
		wstrMessage = L"WM_LBUTTONUP";
		break;
	case WM_LBUTTONDBLCLK:
		wstrMessage = L"WM_LBUTTONDBLCLK";
		break;
	case WM_RBUTTONDOWN:
		wstrMessage = L"WM_RBUTTONDOWN";
		break;
	case WM_RBUTTONUP:
		wstrMessage = L"WM_RBUTTONUP";
		break;
	case WM_RBUTTONDBLCLK:
		wstrMessage = L"WM_RBUTTONDBLCLK";
		break;
	default:
		break;
	}
	if (wstrMessage.empty())
	{
		return;
	}
	std::string strMessage;
	TransCode::Unicode_to_UTF8(wstrMessage.c_str(), wstrMessage.length(), strMessage);
	for (std::vector<int>::iterator iter = m_vecCallback.begin(); iter != m_vecCallback.end(); ++iter)
	{
		int nTop = lua_gettop(m_pLuaState);
		int nRef = *iter;
		lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, nRef);
		lua_pushinteger(m_pLuaState, uID);
		lua_pushstring(m_pLuaState, strMessage.c_str());
		lua_pcall(m_pLuaState, 2, 0, 0);
		lua_settop(m_pLuaState, nTop);
	}
}
