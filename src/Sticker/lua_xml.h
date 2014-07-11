#ifndef _LUA_XML_H_
#define _LUA_XML_H_

#include <windows.h>
#include <XLUE.h>
#include <XLGraphic.h>
#include <XLLuaRuntime.h>
#include "tinyxml2.h"

#define XLUE_LUAXML_CLASSNAME "Sticker.Helper.Xml.Class"
#define XLUE_LUAXML_OBJNAME "Sticker.Helper.Xml"

class LuaXML
{
public:
	LuaXML();
	~LuaXML();

	static long RegisterObject(XL_LRT_ENV_HANDLE hEnv);
private:
	static void* __stdcall GetObject(void* ud);
	static tinyxml2::XMLDocument* GetXmlDocObjFromLuaState(lua_State* luaState);

public:
	static int GetXml(lua_State* luaState);
	static int SetXml(lua_State* luaState);

private:
	static bool ConvertXmlElemToLuaTable(tinyxml2::XMLElement *pElem, lua_State* luaState);
};

#endif