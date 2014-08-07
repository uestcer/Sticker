#include "stdafx.h"
#include <shlwapi.h>

#include "lua_filesystem.h"
#include "lua_xml.h"
#include "lua_notifyicon.h"
#include "lua_stickercore.h"

bool InitXLUE();
void UninitXLUE();
bool LoadXAR();

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	if(!InitXLUE())
	{
		MessageBoxW(NULL,L"��ʼ��XLUE ʧ��!",L"����",MB_OK);
		return 1;
	}

	if(!LoadXAR())
	{
		MessageBoxW(NULL,L"Load XARʧ��!",L"����",MB_OK);
		return 1;
	}

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UninitXLUE();

	return (int) msg.wParam;
}


int __stdcall LuaErrorHandle(lua_State* luaState,const wchar_t* pExtInfo,const wchar_t* wcszLuaErrorString,PXL_LRT_ERROR_STACK pStackInfo)
{
	static bool s_bEnter = false;
	if (!s_bEnter)
	{
		s_bEnter = true;
		std::wstring str = wcszLuaErrorString ? wcszLuaErrorString : L"";
		str += L"@";
		str += pExtInfo ? pExtInfo : L"";
		int ret = MessageBoxW(0,str.c_str(),L"�ű����󣬵��OK�鿴��ջ",MB_ICONERROR | MB_OKCANCEL);

		if (ret == IDOK && pStackInfo->logs != NULL)
		{
			std::string callstack;
			const char* stack = NULL;
			while (XLLRT_RESULT_SUCCESS == XLLRT_DebugLogsPopNextLog(&stack, pStackInfo->logs) && stack != NULL)
			{
				callstack.append(stack);
			}
			MessageBoxA(0,callstack.c_str(),"��ջ", MB_ICONERROR | MB_OK);
		}
		s_bEnter = false;
	}
	return 0;
}



bool InitXLUE()
{
	//��ʼ��ͼ�ο�
	XLGraphicParam param;
	XL_PrepareGraphicParam(&param);
	long result = XL_InitGraphicLib(&param);
	//��ʼ��XLUE,�⺯����һ�����ϳ�ʼ������
	//����˳�ʼ��Lua����,��׼����,XLUELoader�Ĺ���
	XLFS_Init();
	XLUE_InitLoader(NULL);

	//����һ���򵥵Ľű�������ʾ
	XLLRT_ErrorHandle(LuaErrorHandle);

	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	LuaFileSystem::RegisterObject(hEnv);
	LuaXML::RegisterObject(hEnv);
	LuaNotifyIcon::RegisterObject(hEnv);
	LuaStickerCore::RegisterObject(hEnv);
	XLLRT_ReleaseEnv(hEnv);

	return true; 
}

void UninitXLUE()
{
	//�˳�����
	XLUE_Uninit(NULL);
	XLUE_UninitLuaHost(NULL);
	XL_UnInitGraphicLib();
	XLUE_UninitHandleMap(NULL);
}

bool LoadXAR()
{

	WCHAR strCurDir[MAX_PATH];
	ZeroMemory(strCurDir, sizeof(strCurDir));
	GetCurrentDirectoryW(MAX_PATH, strCurDir);
	PathAppend(strCurDir, L"..\\..\\xar");

	long result = 0;
	//����XAR������·��
	result = XLUE_AddXARSearchPath(strCurDir);
	//������XAR,��ʱ��ִ�и�XAR�������ű�onload.lua
	result = XLUE_LoadXAR("BoltTest");
	if(result != 0)
	{
		return false;
	}
	return true;
}