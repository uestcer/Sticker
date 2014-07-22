#include "notifyicon.h"

NotifyIcon::NotifyIcon()
{
	m_hWnd = NULL;
	m_pNotifyIconData = NULL;
}

INotifyIconCallback* NotifyIcon::m_pCallBack = NULL;

NotifyIcon::~NotifyIcon()
{
	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
	}
	if (m_pNotifyIconData)
	{
		delete m_pNotifyIconData;
	}
}

void NotifyIcon::AddIcon( HICON hIcon, PCTSTR pszTipsText )
{
	if (m_hWnd == NULL)
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
		m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
			pszClassName, pszWndName, 
			WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL);
	}

	if (m_hWnd == NULL || hIcon == NULL)
	{
		return;
	}
	
	if (m_pNotifyIconData == NULL)
	{
		m_pNotifyIconData = new NOTIFYICONDATA;
		ZeroMemory(m_pNotifyIconData, sizeof(NOTIFYICONDATA));
	}
	m_pNotifyIconData->hWnd = m_hWnd;
	m_pNotifyIconData->uID = 0;
	m_pNotifyIconData->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_pNotifyIconData->uCallbackMessage = WM_USER;
	m_pNotifyIconData->hIcon = hIcon;
	if (pszTipsText != NULL)
	{
		lstrcpy(m_pNotifyIconData->szTip, pszTipsText);
	}
	Shell_NotifyIcon(NIM_ADD, m_pNotifyIconData);
}

void NotifyIcon::DelIcon()
{
	if (m_pNotifyIconData)
	{
		Shell_NotifyIcon(NIM_DELETE, m_pNotifyIconData);
		m_pNotifyIconData = NULL;
	}
}

void NotifyIcon::SetIcon( HICON hIcon, PCTSTR pszTipsText )
{
	DelIcon();
	AddIcon(hIcon, pszTipsText);
}

void NotifyIcon::Attach( INotifyIconCallback* pCallback )
{
	m_pCallBack = pCallback;
}

void NotifyIcon::Detach()
{
	m_pCallBack = NULL;
}

LRESULT CALLBACK NotifyIcon::WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	UINT WM_TASKBARCREATED;
	WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));

	switch(message)
	{
	case WM_CREATE:
		break;
	case WM_USER:
		if (lParam == WM_LBUTTONDOWN || lParam == WM_LBUTTONUP || lParam == WM_LBUTTONDBLCLK ||
			lParam == WM_RBUTTONDOWN || lParam == WM_RBUTTONUP || lParam == WM_RBUTTONDBLCLK)
		{
			if (m_pCallBack)
			{
				m_pCallBack->OnNotifyIcon((UINT)lParam);
			}
		}
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