#include "lua_xml.h"
#include <string>
#include <map>

static XLLRTGlobalAPI LuaXmlFunctions[] = {
	{"GetXml", LuaXML::GetXml},
	{"SetXml", LuaXML::SetXml},
	{NULL,NULL}
};

LuaXML::LuaXML()
{

}

LuaXML::~LuaXML()
{

}

long LuaXML::RegisterObject( XL_LRT_ENV_HANDLE hEnv )
{
	if (hEnv == NULL)
	{
		return XLLRT_RESULT_ENV_INVALID;
	}

	XLLRTObject obj;
	obj.ClassName = XLUE_LUAXML_CLASSNAME;
	obj.ObjName = XLUE_LUAXML_OBJNAME;
	obj.MemberFunctions = LuaXmlFunctions;
	obj.userData = NULL;
	obj.pfnGetObject = LuaXML::GetObject;

	long lRet = XLLRT_RegisterGlobalObj(hEnv, obj);
	return lRet;
}

void* __stdcall LuaXML::GetObject( void* ud )
{
	static tinyxml2::XMLDocument doc;
	return &doc;
}

tinyxml2::XMLDocument* LuaXML::GetXmlDocObjFromLuaState( lua_State* luaState )
{
	tinyxml2::XMLDocument** ppDoc = (tinyxml2::XMLDocument**)luaL_checkudata(luaState, 1, XLUE_LUAXML_CLASSNAME);
	if (ppDoc != NULL)
	{
		return *ppDoc;
	}
	return NULL;
}

bool LuaXML::ConvertXmlElemToLuaTable( tinyxml2::XMLElement *pElem, lua_State* luaState )
{
	if (pElem == NULL || pElem->NoChildren())
	{
		return false;
	}

	bool bHasRepeatItem = false;
	std::map<std::string, unsigned int> countMap;
	tinyxml2::XMLElement* pCountElem = pElem->FirstChildElement();
	while(pCountElem)
	{
		const char* strName = pCountElem->Name();
		std::map<std::string, unsigned int>::iterator iter = countMap.find(strName);
		if (iter != countMap.end())
		{
			bHasRepeatItem = true;
			break;
		}
		else
		{
			countMap.insert(std::pair<std::string, unsigned int>(strName, 0));
		}
		pCountElem = pCountElem->NextSiblingElement();
	}

	if (bHasRepeatItem)
	{
		// 如果有重复的 item， 用数组的方式存储
		lua_newtable(luaState);
		tinyxml2::XMLElement* iterElem = pElem->FirstChildElement();
		unsigned int i = 0;
		while(iterElem)
		{
			const char* strText = iterElem->GetText();
			if (strText)
			{
				lua_pushstring(luaState, strText);
			}
			else
			{
				ConvertXmlElemToLuaTable(iterElem, luaState);
			}
			lua_rawseti(luaState, -2, i+1);
			i++;
			iterElem = iterElem->NextSiblingElement();
		}
	}
	else
	{
		lua_newtable(luaState);
		tinyxml2::XMLElement* iterElem = pElem->FirstChildElement();
		while(iterElem)
		{
			const char* strName = iterElem->Name();
			const char* strText = iterElem->GetText();
			if (strText)
			{
				lua_pushstring(luaState, strName);
				lua_pushstring(luaState, strText);
			}
			else
			{
				lua_pushstring(luaState, strName);
				ConvertXmlElemToLuaTable(iterElem, luaState);
			}
			lua_settable(luaState, -3);
			iterElem = iterElem->NextSiblingElement();
		}
	}

	return true;
}

bool LuaXML::ConvertLuaTableToXmlElem( tinyxml2::XMLElement *pElem, lua_State* luaState )
{
	return true;
}

int LuaXML::GetXml( lua_State* luaState )
{
	int bRet = 0;
	do
	{
		tinyxml2::XMLDocument* pDoc = GetXmlDocObjFromLuaState(luaState);
		if (pDoc == NULL)
		{
			break;
		}
		const char* strPath = luaL_checkstring(luaState, 2);
		if (strPath == NULL)
		{
			break;
		}
		tinyxml2::XMLError err = pDoc->LoadFile(strPath);
		if (err != tinyxml2::XML_NO_ERROR)
		{
			break;
		}
		if (pDoc->RootElement() == NULL || pDoc->RootElement()->Name() == NULL)
		{
			break;
		}
		lua_newtable(luaState);
		const char* strName = pDoc->RootElement()->Name();
		lua_pushstring(luaState, strName);
		ConvertXmlElemToLuaTable(pDoc->RootElement(), luaState);
		lua_settable(luaState, -3);
		
		pDoc->Clear();
		bRet = 1;
	}while(false);
	return bRet;
}

int LuaXML::SetXml( lua_State* luaState )
{
	int bRet = 0;
	do 
	{
		tinyxml2::XMLDocument* pDoc = GetXmlDocObjFromLuaState(luaState);
		if (pDoc == NULL)
		{
			break;
		}
		const char* strPath = luaL_checkstring(luaState, 2);
		if (strPath == NULL)
		{
			break;
		}
		if (lua_gettop(luaState) != 3 || lua_istable(luaState, -1) != 1)
		{
			break;
		}
		int nIndex = lua_gettop(luaState);
		lua_pushnil(luaState);
		while(0 != lua_next(luaState, nIndex))
		{
			int nKeyType = lua_type(luaState, -2);
			int nValueType = lua_type(luaState, -1);
			if (nKeyType == LUA_TNUMBER)
			{
				int nKey = luaL_checkint(luaState, -2);
				nKey ++;
			}
			else if (nKeyType == LUA_TSTRING)
			{
				const char* strKey = luaL_checkstring(luaState, -2);
			}
			const char* strValue = luaL_checkstring(luaState, -1);
			lua_pop(luaState, 1);
		}
		pDoc->Clear();
		tinyxml2::XMLElement* pElem = pDoc->NewElement("data");
		pElem->SetName("data");
		pElem->SetText("hello xml");
		pDoc->LinkEndChild(pElem);
		pDoc->SaveFile(strPath);

	} while (false);
	return bRet;
}