#ifndef _LUA_FILESYSTEM_
#define _LUA_FILESYSTEM_

#define XLUE_LUAFILESYSTEM_CLASSNAME "Sticker.Helper.FileSystem.Class"
#define XLUE_LUAFILESYSTEM_OBJNAME "Sticker.Helper.FileSystem"

class FileSystem;

class LuaFileSystem
{
public:
	LuaFileSystem();
	~LuaFileSystem();

	static long RegisterObject(XL_LRT_ENV_HANDLE hEnv);
private:
	static void* __stdcall GetObject(void* ud);
	static FileSystem* GetFileSystemObjFromLuaState(lua_State* luaState);

public:
	static int GetFileSize(lua_State* luaState);
	static int IsFileExist(lua_State* luaState);
	static int RemoveFile(lua_State* luaState);
	static int MakeDir(lua_State* luaState);
	static int DeleteDir(lua_State* luaState);
	static int DeleteDirTree(lua_State* luaState);
	static int IsDirExist(lua_State* luaState);
	static int GetDirList(lua_State* luaState);
	static int GetFileList(lua_State* luaState);
	static int GetCurrentWorkingDir(lua_State* luaState);
	static int SetCurrentWoringDir(lua_State* luaState);
	static int GetCurrentExeDir(lua_State* luaState);
	static int GetCurrentExeName(lua_State* luaState);
	static int GetFileName(lua_State* luaState);
	static int GetPathDir(lua_State* luaState);
	static int GetPathDrive(lua_State* luaState);
	static int GetFileExt(lua_State* luaState);
	static int GetFileMainName(lua_State* luaState);
	static int GetUpperDir(lua_State* luaState);
	static int PathCombine(lua_State* luaState);
	static int ReadFile(lua_State* luaState);
	static int WriteFile(lua_State* luaState);

};

#endif