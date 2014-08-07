#include "stdafx.h"
#include <Shlwapi.h>
#include "filesystem.h"

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{

}

__int64 FileSystem::GetFileSize( std::wstring path )
{
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	LARGE_INTEGER li;
	BOOL bRet = GetFileSizeEx(hFile,&li);
	CloseHandle(hFile);
	if (!bRet)
	{
		return -1;
	}
	return li.QuadPart;
}

bool FileSystem::IsFileExist( std::wstring path )
{
	BOOL bRet = PathFileExists(path.c_str());
	return bRet == TRUE;
}

bool FileSystem::RemoveFile( std::wstring path )
{
	BOOL bRet = ::DeleteFile(path.c_str());
	return bRet != 0;
}

bool FileSystem::MakeDir( std::wstring dir )
{
	BOOL bRet = CreateDirectory(dir.c_str(), NULL);
	return bRet == TRUE;
}

bool FileSystem::DeleteDir( std::wstring dir )
{
	BOOL bRet = RemoveDirectory(dir.c_str());
	return bRet != 0;
}

bool FileSystem::DeleteDirTree( std::wstring dir )
{
	BOOL bRet = PathIsDirectory(dir.c_str());
	if (bRet != FILE_ATTRIBUTE_DIRECTORY)
	{
		return false;
	}

	WCHAR buf[MAX_PATH] = {0};
	::PathCombine(buf, dir.c_str(), L"*");

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(buf, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	BOOL bFlag = TRUE;
	while(bFlag)
	{
		std::wstring strFileName = fd.cFileName;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFileName != L"." && strFileName != L"..")
			{
				WCHAR strSubDir[MAX_PATH] = {0};
				::PathCombine(strSubDir, dir.c_str(), strFileName.c_str());
				this->DeleteDirTree(strSubDir);
			}
		}
		else
		{
			WCHAR strFilePath[MAX_PATH] = {0};
			::PathCombine(strFilePath, dir.c_str(), strFileName.c_str());
			::DeleteFile(strFilePath);
		}
		bFlag = FindNextFile(hFind, &fd);
	}
	FindClose(hFind);
	bRet = RemoveDirectory(dir.c_str());
	return bRet != 0;
}

bool FileSystem::IsDirExist( std::wstring dir )
{	
	BOOL bRet = PathIsDirectory(dir.c_str());
	return bRet == FILE_ATTRIBUTE_DIRECTORY;
}

bool FileSystem::GetDirList( std::wstring dir, std::vector<std::wstring>& dirList)
{
	BOOL bRet = PathIsDirectory(dir.c_str());
	if (bRet != FILE_ATTRIBUTE_DIRECTORY)
	{
		return false;
	}

	WCHAR buf[MAX_PATH] = {0};
	::PathCombine(buf, dir.c_str(), L"*");

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(buf, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	BOOL bFlag = TRUE;
	while(bFlag)
	{
		std::wstring strFileName = fd.cFileName;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && strFileName != L"." && strFileName != L"..")
		{
			WCHAR buf[MAX_PATH] = {0};
			::PathCombine(buf, dir.c_str(), strFileName.c_str());
			dirList.push_back(buf);
		}
		bFlag = FindNextFile(hFind, &fd);
	}
	FindClose(hFind);
	return true;
}

bool FileSystem::GetFileList( std::wstring dir, std::vector<std::wstring>& fileList )
{
	BOOL bRet = PathIsDirectory(dir.c_str());
	if (bRet != FILE_ATTRIBUTE_DIRECTORY)
	{
		return false;
	}

	WCHAR buf[MAX_PATH] = {0};
	::PathCombine(buf, dir.c_str(), L"*");

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(buf, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	BOOL bFlag = TRUE;
	while(bFlag)
	{
		std::wstring strFileName = fd.cFileName;
		if ( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			WCHAR buf[MAX_PATH] = {0};
			::PathCombine(buf, dir.c_str(), strFileName.c_str());
			fileList.push_back(buf);
		}
		bFlag = FindNextFile(hFind, &fd);
	}
	FindClose(hFind);
	return true;
}

std::wstring FileSystem::GetCurrentWorkingDir()
{
	WCHAR buf[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, buf);
	return buf;
}

bool FileSystem::SetCurrentWoringDir( std::wstring dir )
{
	BOOL bRet = SetCurrentDirectory(dir.c_str());
	return bRet != 0;
}

std::wstring FileSystem::GetCurrentExeDir()
{
	WCHAR buf[MAX_PATH] = {0};
	GetModuleFileName(NULL, buf, MAX_PATH);
	::PathCombine(buf, buf, L"..");
	return buf;
}

std::wstring FileSystem::GetCurrentExeName()
{
	WCHAR buf[MAX_PATH] = {0};
	GetModuleFileName(NULL, buf, MAX_PATH);
	PathStripPath(buf);
	return buf;
}

std::wstring FileSystem::GetFileName( std::wstring path )
{
	WCHAR buf[MAX_PATH] = {0};
	StrCpy(buf, path.c_str());
	PathStripPath(buf);
	return buf;
}

std::wstring FileSystem::GetPathDir( std::wstring path )
{
	WCHAR buf[MAX_PATH] = {0};
	StrCpy(buf, path.c_str());
	BOOL bRet = PathRemoveFileSpec(buf);
	if (bRet == 0)
	{
		return L"";
	}
	else
	{
		return buf;
	}
}

std::wstring FileSystem::GetPathDrive( std::wstring path )
{
	WCHAR strDrive[_MAX_DRIVE];
	errno_t err = _wsplitpath_s(path.c_str(), strDrive, _MAX_DRIVE, NULL, 0, NULL, 0, NULL, 0);
	if (err == 0)
	{
		return strDrive;
	}
	else
	{
		return L"";
	}
}

std::wstring FileSystem::GetFileExt( std::wstring path )
{
	WCHAR strExt[_MAX_EXT];
	errno_t err = _wsplitpath_s(path.c_str(), NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT);
	if (err == 0)
	{
		return strExt;
	}
	else
	{
		return L"";
	}
}

std::wstring FileSystem::GetFileMainName( std::wstring path )
{
	WCHAR strFileName[_MAX_FNAME];
	errno_t err = _wsplitpath_s(path.c_str(), NULL, 0, NULL, 0, strFileName, _MAX_FNAME, NULL, 0);
	if (err == 0)
	{
		return strFileName;
	}
	else
	{
		return L"";
	}
}

std::wstring FileSystem::GetUpperDir( std::wstring path )
{
	return CombinePath(path, L"..");
}

std::wstring FileSystem::CombinePath( std::wstring dir, std::wstring name )
{
	for (std::wstring::iterator iter = dir.begin(); iter != dir.end(); ++iter)
	{
		if (*iter == '/')
		{
			*iter = '\\';
		}
	}
	for (std::wstring::iterator iter = name.begin(); iter != name.end(); ++iter)
	{
		if (*iter == '/')
		{
			*iter = '\\';
		}
	}
	WCHAR strPath[MAX_PATH] = {0};
	WCHAR* pRet = ::PathCombine(strPath, dir.c_str(), name.c_str());
	if (pRet)
	{
		return strPath;
	}
	else
	{
		return L"";
	}
}
