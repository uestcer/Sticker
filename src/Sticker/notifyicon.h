#ifndef _NOTIFYICON_H_
#define _NOTIFYICON_H_

#include <Windows.h>

class INotifyIconCallback
{
public:
	virtual void OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam)=0;
};

class NotifyIcon
{
public:
	NotifyIcon();
	~NotifyIcon();

	void Init();
	void UnInit();
	void SetIcon(HICON hIcon);
	void SetIcon(PCTSTR pszIconPath);
	void Show();
	void Hide();

	unsigned Attach(INotifyIconCallback* pCallback);
	void Detach(unsigned nCookie);

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif