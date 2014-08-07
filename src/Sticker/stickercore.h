#ifndef _STICKERCORE_H_
#define _STICKERCORE_H_

typedef void (*StickerCoreCallback) ();

class StickerCore
{
public:
	static int ExitSticker();
	static UINT AttachListener(StickerCoreCallback callbackFunc);
	static void DetachListener(UINT uCookie);

private:
	static std::map<UINT, StickerCoreCallback> m_mapCallback;
	static UINT m_uCookieCount;
};

#endif