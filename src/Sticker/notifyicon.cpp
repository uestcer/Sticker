#include "stdafx.h"
#include "notifyicon.h"

HWND NotifyIcon::m_hWnd = NULL;
UINT NotifyIcon::m_uCookieCount = 1;
std::map<UINT,NotifyIconCallback> NotifyIcon::m_mapCallback;
UINT NotifyIcon::m_uIDCount = 1;
std::map<UINT,NOTIFYICONDATA> NotifyIcon::m_mapNotifyData;

UINT NotifyIcon::AddIcon( HICON hIcon, const WCHAR* szTip )
{
	// ´´½¨´°¿Ú
	LPCTSTR pszClassName = TEXT("NotifyIcon_ClassName");
	LPCTSTR pszWndName = TEXT("NotifyIcon_WndName");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wndClass;
	if (GetClassInfo(hInstance, pszClassName, &wndClass) == 0)
	{
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
		if (RegisterClass(&wndClass) == NULL)
		{
			return NULL;
		}
	}
	if (FindWindow(pszClassName, pszWndName) == NULL)
	{
		m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
			pszClassName, pszWndName, 
			WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL);
		if (m_hWnd == NULL)
		{
			return NULL;
		}
	}

	UINT uID = m_uIDCount;
	m_uIDCount++;

	NOTIFYICONDATA nd;
	nd.cbSize = sizeof(nd);
	nd.hWnd = m_hWnd;
	nd.uID = uID;
	nd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nd.uCallbackMessage = WM_USER;
	nd.hIcon = hIcon;
	if (szTip != NULL && lstrlen(szTip) < 127)
	{
		lstrcpy(nd.szTip, szTip);
	}
	if(Shell_NotifyIcon(NIM_ADD, &nd) == TRUE)
	{
		m_mapNotifyData.insert(std::make_pair(uID, nd));
		return uID;
	}

	return NULL;
}

VOID NotifyIcon::DelIcon( UINT uID )
{
	if (m_hWnd == NULL)
	{
		return;
	}
	std::map<UINT, NOTIFYICONDATA>::iterator iter = m_mapNotifyData.find(uID);
	if (iter != m_mapNotifyData.end())
	{
		Shell_NotifyIcon(NIM_DELETE, &(iter->second));
		m_mapNotifyData.erase(iter);
	}
}

VOID NotifyIcon::ModIcon( UINT uID, HICON hIcon, const WCHAR* szTip )
{
	if (m_hWnd == NULL)
	{
		return;
	}
	std::map<UINT, NOTIFYICONDATA>::iterator iter = m_mapNotifyData.find(uID);
	if (iter != m_mapNotifyData.end())
	{
		if (hIcon != NULL)
		{
			iter->second.hIcon = hIcon;
		}
		if (szTip && lstrlen(szTip) < 127)
		{
			lstrcpy(iter->second.szTip, szTip);
		}
		else
		{
			lstrcpy(iter->second.szTip, L"");
		}
		Shell_NotifyIcon(NIM_MODIFY, &(iter->second));
	}
}

UINT NotifyIcon::Attach( NotifyIconCallback pCallback )
{
	if (pCallback == NULL)
	{
		return NULL;
	}
	UINT uCookie = m_uCookieCount;
	m_uCookieCount ++;

	m_mapCallback.insert(std::make_pair(uCookie, pCallback));
	return uCookie;
}

VOID NotifyIcon::Detach( UINT uCookie )
{
	std::map<UINT, NotifyIconCallback>::iterator iter = m_mapCallback.find(uCookie);
	if (iter != m_mapCallback.end())
	{
		m_mapCallback.erase(iter);
	}
}

LRESULT CALLBACK NotifyIcon::WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		break;
	case WM_USER:
		for (std::map<UINT, NotifyIconCallback>::iterator iter = m_mapCallback.begin(); iter != m_mapCallback.end(); ++iter)
		{
			NotifyIconCallback pCallback = iter->second;
			pCallback(wParam, lParam);        // wParam --> uID, lParam --> message
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}