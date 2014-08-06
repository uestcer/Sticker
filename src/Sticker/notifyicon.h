#ifndef _NOTIFYICON_H_
#define _NOTIFYICON_H_
#include <windows.h>
#include <shellapi.h>
#include <map>

typedef VOID (*NotifyIconCallback) (UINT uID, UINT message);

class NotifyIcon
{
public:
	static UINT AddIcon(HICON hIcon, WCHAR* szTip);
	static VOID DelIcon(UINT uID);
	static VOID ModIcon(UINT uID, HICON hIcon, WCHAR* szTip);
	static UINT Attach(NotifyIconCallback pCallback);
	static VOID Detach(UINT uCookie);

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HWND m_hWnd;
	static UINT m_uCookieCount;
	static std::map<UINT,NotifyIconCallback> m_mapCallback;
	static UINT m_uIDCount;
	static std::map<UINT,NOTIFYICONDATA> m_mapNotifyData;
};

#endif