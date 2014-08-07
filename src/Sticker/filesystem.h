#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

class FileSystem
{
public:
	FileSystem();
	~FileSystem();
	
	__int64 GetFileSize(std::wstring path);
	bool IsFileExist(std::wstring path);
	bool RemoveFile(std::wstring path);
	bool MakeDir(std::wstring dir);
	bool DeleteDir(std::wstring dir);
	bool DeleteDirTree(std::wstring dir);
	bool IsDirExist(std::wstring dir);
	bool GetDirList( std::wstring dir, std::vector<std::wstring>& dirList);
	bool GetFileList(std::wstring dir, std::vector<std::wstring>& fileList);
	std::wstring GetCurrentWorkingDir();
	bool SetCurrentWoringDir(std::wstring dir);
	std::wstring GetCurrentExeDir();
	std::wstring GetCurrentExeName();
	std::wstring GetFileName(std::wstring path);
	std::wstring GetPathDir(std::wstring path);
	std::wstring GetPathDrive(std::wstring path);
	std::wstring GetFileExt(std::wstring path);
	std::wstring GetFileMainName(std::wstring path);
	std::wstring GetUpperDir(std::wstring path);
	std::wstring CombinePath(std::wstring dir, std::wstring name);

};

#endif