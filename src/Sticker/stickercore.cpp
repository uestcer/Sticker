#include "stdafx.h"
#include "stickercore.h"

std::map<UINT, StickerCoreCallback> StickerCore::m_mapCallback;
UINT StickerCore::m_uCookieCount = 1;

int StickerCore::ExitSticker()
{
	std::map<UINT, StickerCoreCallback>::iterator iter = m_mapCallback.begin();
	for (iter; iter != m_mapCallback.end(); ++iter)
	{
		StickerCoreCallback pCallbackFunc = iter->second;
		pCallbackFunc();
	}
	PostMessage(NULL, WM_QUIT, 0, 0);
	return 1;
}

UINT StickerCore::AttachListener( StickerCoreCallback callbackFunc )
{
	if (callbackFunc == NULL)
	{
		return 0;
	}
	UINT uCookie = m_uCookieCount;
	m_uCookieCount++;
	m_mapCallback.insert(std::make_pair(uCookie, callbackFunc));
	
	return uCookie;
}

void StickerCore::DetachListener( UINT uCookie )
{
	if (uCookie == 0)
	{
		return;
	}
	std::map<UINT, StickerCoreCallback>::iterator iter = m_mapCallback.find(uCookie);
	if (iter != m_mapCallback.end())
	{
		m_mapCallback.erase(iter);
	}
	if (m_mapCallback.size() == 0)
	{
		m_uCookieCount = 1;
	}
}

