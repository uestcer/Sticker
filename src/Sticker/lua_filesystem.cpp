#include "filesystem.h"
#include "lua_filesystem.h"
#include "transcode.h"

static XLLRTGlobalAPI LuaFileSystemFunctions[] = {
	{"GetFileSize", LuaFileSystem::GetFileSize},
	{"IsFileExist", LuaFileSystem::IsFileExist},
	{"RemoveFile", LuaFileSystem::RemoveFile},
	{"MakeDir", LuaFileSystem::MakeDir},
	{"DeleteDir", LuaFileSystem::DeleteDir},
	{"DeleteDirTree", LuaFileSystem::DeleteDirTree},
	{"IsDirExist", LuaFileSystem::IsDirExist},
	{"GetDirList", LuaFileSystem::GetDirList},
	{"GetFileList", LuaFileSystem::GetFileList},
	{"GetCurrentWorkingDir", LuaFileSystem::GetCurrentWorkingDir},
	{"SetCurrentWoringDir", LuaFileSystem::SetCurrentWoringDir},
	{"GetCurrentExeDir", LuaFileSystem::GetCurrentExeDir},
	{"GetCurrentExeName", LuaFileSystem::GetCurrentExeName},
	{"GetFileName", LuaFileSystem::GetFileName},
	{"GetPathDir", LuaFileSystem::GetPathDir},
	{"GetPathDrive", LuaFileSystem::GetPathDrive},
	{"GetFileExt", LuaFileSystem::GetFileExt},
	{"GetFileMainName", LuaFileSystem::GetFileMainName},
	{"GetUpperDir", LuaFileSystem::GetUpperDir},
	{"PathCombine", LuaFileSystem::PathCombine},
	{NULL,NULL}
};

LuaFileSystem::LuaFileSystem()
{

}

LuaFileSystem::~LuaFileSystem()
{

}

long LuaFileSystem::RegisterObject( XL_LRT_ENV_HANDLE hEnv )
{
	if(hEnv == NULL)
	{
		return XLLRT_RESULT_ENV_INVALID;
	}

	XLLRTObject theObject;
	theObject.ClassName = XLUE_LUAFILESYSTEM_CLASSNAME;
	theObject.MemberFunctions = LuaFileSystemFunctions;
	theObject.ObjName = XLUE_LUAFILESYSTEM_OBJNAME;
	theObject.userData = NULL;
	theObject.pfnGetObject = LuaFileSystem::GetObject;

	long lRet = XLLRT_RegisterGlobalObj(hEnv,theObject);
	return lRet;
}

void* __stdcall LuaFileSystem::GetObject(void* ud)
{
	static FileSystem s_fileSystemObj;
	return (void*)&s_fileSystemObj;
}

FileSystem* LuaFileSystem::GetFileSystemObjFromLuaState( lua_State* luaState )
{
	FileSystem** lplpLuaObj = (FileSystem**)luaL_checkudata(luaState,1, XLUE_LUAFILESYSTEM_CLASSNAME);   
	if(lplpLuaObj != NULL)
	{
		return *lplpLuaObj;
	}

	return NULL;   
}

int LuaFileSystem::GetFileSize( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);
	if (!wstrPath.empty())
	{
		__int64 nSize = pFileSystem->GetFileSize(wstrPath);
		lua_pushnumber(luaState, (lua_Number)nSize);
		return 1;
	}
	return 0;
}

int LuaFileSystem::IsFileExist( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);
	if (!wstrPath.empty())
	{
		bool bExist = pFileSystem->IsFileExist(wstrPath);
		lua_pushboolean(luaState, bExist);
		return 1;
	}
	return 0;
}

int LuaFileSystem::RemoveFile( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);
	if (!wstrPath.empty())
	{
		bool bRet = pFileSystem->RemoveFile(wstrPath);
		lua_pushboolean(luaState, bRet);
		return 1;
	}
	return 0;
}

int LuaFileSystem::MakeDir( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	std::wstring wstrDir;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	if (!wstrDir.empty())
	{
		bool bRet = pFileSystem->MakeDir(wstrDir);
		lua_pushboolean(luaState, bRet);
		return 1;
	}
	return 0;
}

int LuaFileSystem::DeleteDir( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	std::wstring wstrDir;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	if (!wstrDir.empty())
	{
		bool bRet = pFileSystem->DeleteDir(wstrDir);
		lua_pushboolean(luaState, bRet);
		return 1;
	}
	return 0;
}

int LuaFileSystem::DeleteDirTree( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	std::wstring wstrDir;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	if (!wstrDir.empty())
	{
		bool bRet = pFileSystem->DeleteDirTree(wstrDir);
		lua_pushboolean(luaState, bRet);
		return 1;
	}
	return 0;
}

int LuaFileSystem::IsDirExist( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	std::wstring wstrDir;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	if (!wstrDir.empty())
	{
		bool bRet = pFileSystem->IsDirExist(wstrDir);
		lua_pushboolean(luaState, bRet);
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetDirList( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	std::wstring wstrDir;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	if (!wstrDir.empty())
	{
		std::vector<std::wstring> dirList;
		bool bRet = pFileSystem->GetDirList(wstrDir, dirList);
		if (bRet == true)
		{
			lua_newtable(luaState);
			for (unsigned i=0; i<dirList.size(); ++i)
			{
				std::string strDirItem;
				TransCode::Unicode_to_UTF8(dirList.at(i).c_str(), dirList.at(i).length(), strDirItem);
				lua_pushstring(luaState, strDirItem.c_str());
				lua_rawseti(luaState, -2, i+1);
			}
			return 1;
		}
	}
	return 0;
}

int LuaFileSystem::GetFileList( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	std::wstring wstrDir;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	if (!wstrDir.empty())
	{
		std::vector<std::wstring> fileList;
		bool bRet = pFileSystem->GetFileList(wstrDir, fileList);
		if (bRet == true)
		{
			lua_newtable(luaState);
			for (unsigned i=0; i<fileList.size(); ++i)
			{
				std::wstring wstrFileItem = fileList.at(i);
				std::string strFileItem;
				TransCode::Unicode_to_UTF8(wstrFileItem.c_str(), wstrFileItem.length(), strFileItem);
				lua_pushstring(luaState, strFileItem.c_str());
				lua_rawseti(luaState, -2, i+1);
			}
			return 1;
		}
	}
	return 0;
}

int LuaFileSystem::GetCurrentWorkingDir( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	std::wstring wstrCurDir = pFileSystem->GetCurrentWorkingDir();
	std::string strCurDir;
	TransCode::Unicode_to_UTF8(wstrCurDir.c_str(), wstrCurDir.length(), strCurDir);
	if (!strCurDir.empty())
	{
		lua_pushstring(luaState, strCurDir.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::SetCurrentWoringDir( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	std::wstring wstrDir;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	if (!wstrDir.empty())
	{
		bool bRet = pFileSystem->SetCurrentWoringDir(wstrDir);
		lua_pushboolean(luaState, bRet);
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetCurrentExeDir( lua_State* luaState )
{	
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	std::wstring wstrExeDir = pFileSystem->GetCurrentExeDir();
	std::string strExeDir;
	TransCode::Unicode_to_UTF8(wstrExeDir.c_str(), wstrExeDir.length(), strExeDir);
	if (!strExeDir.empty())
	{
		lua_pushstring(luaState, strExeDir.c_str());
		return 1;
	}
	return 0;
}	

int LuaFileSystem::GetCurrentExeName( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	std::wstring wstrExeName = pFileSystem->GetCurrentExeName();
	std::string strExeName;
	TransCode::Unicode_to_UTF8(wstrExeName.c_str(), wstrExeName.length(), strExeName);
	if (!strExeName.empty())
	{
		lua_pushstring(luaState, strExeName.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetFileName( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);

	std::wstring wstrFileName = pFileSystem->GetFileName(wstrPath);
	std::string strFileName;
	TransCode::Unicode_to_UTF8(wstrFileName.c_str(), wstrFileName.length(), strFileName);
	if (!strFileName.empty())
	{
		lua_pushstring(luaState, strFileName.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetPathDir( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);

	std::wstring wstrPathDir = pFileSystem->GetPathDir(wstrPath);
	std::string strPathDir;
	TransCode::Unicode_to_UTF8(wstrPathDir.c_str(), wstrPathDir.length(), strPathDir);
	if (!strPathDir.empty())
	{
		lua_pushstring(luaState, strPathDir.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetPathDrive( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);

	std::wstring wstrDrive = pFileSystem->GetPathDrive(wstrPath);
	std::string strDrive;
	TransCode::Unicode_to_UTF8(wstrDrive.c_str(), wstrDrive.length(), strDrive);
	if (!strDrive.empty())
	{
		lua_pushstring(luaState, strDrive.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetFileExt( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);

	std::wstring wstrExt = pFileSystem->GetFileExt(wstrPath);
	std::string strExt;
	TransCode::Unicode_to_UTF8(wstrExt.c_str(), wstrExt.length(), strExt);
	if (!strExt.empty())
	{
		lua_pushstring(luaState, strExt.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetFileMainName( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);

	std::wstring wstrMainName = pFileSystem->GetFileMainName(wstrPath);
	std::string strMainName;
	TransCode::Unicode_to_UTF8(wstrMainName.c_str(), wstrMainName.length(), strMainName);
	if (!strMainName.empty())
	{
		lua_pushstring(luaState, strMainName.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::GetUpperDir( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strPath = luaL_checkstring(luaState, 2);
	std::wstring wstrPath;
	TransCode::UTF8_to_Unicode(strPath, strlen(strPath), wstrPath);

	std::wstring wstrUpperDir = pFileSystem->GetFileMainName(wstrPath);
	std::string strUpperDir;
	TransCode::Unicode_to_UTF8(wstrUpperDir.c_str(), wstrUpperDir.length(), strUpperDir);
	if (!strUpperDir.empty())
	{
		lua_pushstring(luaState, strUpperDir.c_str());
		return 1;
	}
	return 0;
}

int LuaFileSystem::PathCombine( lua_State* luaState )
{
	FileSystem* pFileSystem = GetFileSystemObjFromLuaState(luaState);
	const char* strDir = luaL_checkstring(luaState, 2);
	const char* strName = luaL_checkstring(luaState, 3);
	std::wstring wstrDir;
	std::wstring wstrName;
	TransCode::UTF8_to_Unicode(strDir, strlen(strDir), wstrDir);
	TransCode::UTF8_to_Unicode(strName, strlen(strName), wstrName);

	std::wstring wstrPath = pFileSystem->CombinePath(wstrDir, wstrName);
	if (!wstrPath.empty())
	{
		std::string strPath;
		TransCode::Unicode_to_UTF8(wstrPath.c_str(), wstrPath.length(), strPath);
		lua_pushstring(luaState, strPath.c_str());
		return 1;
	}
	return 0;
}


