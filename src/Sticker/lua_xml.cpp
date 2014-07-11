#include "lua_xml.h"

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

	tinyxml2::XMLElement* iterElem = pElem->FirstChildElement();
	while(iterElem)
	{
		const char* strName = iterElem->Name();
		if (iterElem->NoChildren())
		{
			const char* strText = iterElem->GetText();
		}
		else
		{
			ConvertXmlElemToLuaTable(pElem, luaState);
		}
		iterElem = iterElem->NextSiblingElement();
	}
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
		ConvertXmlElemToLuaTable(pDoc->RootElement(), luaState);
		return 1;
	}while(false);
	return bRet;
}

int LuaXML::SetXml( lua_State* luaState )
{
	return 0;
}