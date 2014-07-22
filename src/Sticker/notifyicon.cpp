#include "notifyicon.h"
#include <shellapi.h>


NotifyIcon::NotifyIcon()
{

}

NotifyIcon::~NotifyIcon()
{

}

void NotifyIcon::Init()
{
	LPCTSTR pszClassName = TEXT("Sticker_NotifyIcon_ClassName");
	LPCTSTR pszWndName = TEXT("Sticker_NotifyIcon_WndName");

	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = NotifyIcon::WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = NULL;
	wndClass.lpszClassName = pszClassName;
	wndClass.lpszMenuName = NULL;
	if (!RegisterClass(&wndClass))
	{
		return;
	}
	HWND hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
		pszClassName, pszWndName, 
		WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
}

void NotifyIcon::UnInit()
{

}

void NotifyIcon::SetIcon( HICON hIcon )
{

}

void NotifyIcon::SetIcon( PCTSTR pszIconPath )
{

}

void NotifyIcon::Show()
{

}

void NotifyIcon::Hide()
{

}

unsigned NotifyIcon::Attach( INotifyIconCallback* pCallback )
{
	return 0;
}

void NotifyIcon::Detach( unsigned nCookie )
{
}

LRESULT CALLBACK NotifyIcon::WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	NOTIFYICONDATA nid;
	UINT WM_TASKBARCREATED;
	WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));

	switch(message)
	{
	case WM_CREATE:
		OutputDebugString(L"[Dongyuxxx] WM_CREATE ENTER");
		nid.hWnd = hwnd;
		nid.uID = 0;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = WM_USER;
		nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		lstrcpy(nid.szTip, L"xxx");
		Shell_NotifyIcon(NIM_ADD, &nid);
		break;
	case WM_USER:
		break;
	case WM_DESTROY:
		break;
	default:
		if (message == WM_TASKBARCREATED)
		{
			::SendMessage(hwnd, message, wParam, lParam);
		}
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
