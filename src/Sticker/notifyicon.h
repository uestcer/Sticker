#ifndef _NOTIFYICON_H_
#define _NOTIFYICON_H_

#include <Windows.h>
#include <shellapi.h>

class INotifyIconCallback
{
public:
	virtual void OnNotifyIcon(UINT uMsg)=0;
};

class NotifyIcon
{
public:
	NotifyIcon();
	~NotifyIcon();

	void AddIcon(HICON hIcon, PCTSTR pszTipsText);
	void DelIcon();
	void SetIcon(HICON hIcon, PCTSTR pszTipsText);

	void Attach(INotifyIconCallback* pCallback);
	void Detach();

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;
	NOTIFYICONDATA* m_pNotifyIconData;
	static INotifyIconCallback* m_pCallBack;
};

#endif