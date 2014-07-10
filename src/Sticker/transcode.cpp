#include "transcode.h"
#include <Windows.h>


void TransCode::Unicode_to_UTF8( const wchar_t* in, unsigned len, std::string& out )
{
	size_t outlen = len * 3 + 1;
	char* pbuf = new char[outlen];
	if (NULL == pbuf)
	{
		return;
	}
	memset(pbuf, 0, outlen);
	::WideCharToMultiByte(CP_UTF8, 0, in, len, pbuf, len * 3, NULL, NULL);
	out.assign(pbuf);

	delete[] pbuf;
	pbuf = NULL;
	return;
}

void TransCode::UTF8_to_Unicode( const char* in, unsigned len, std::wstring& out )
{
	wchar_t* pbuf = new wchar_t[len + 1];
	if (NULL == pbuf)
	{
		return;
	}
	size_t outlen = (len + 1) * sizeof(wchar_t);
	memset(pbuf, 0, outlen);
	::MultiByteToWideChar(CP_UTF8, 0, in, len, pbuf, len * sizeof(wchar_t));
	out.assign(pbuf);

	delete[] pbuf;
	pbuf = NULL;
	return;
}

void TransCode::GBK_to_UTF8( const char* in, unsigned len, std::string& out )
{

}

void TransCode::UTF8_to_GBK( const char* in, unsigned len, std::string& out )
{

}

void TransCode::Unicode_to_GBK( const wchar_t* in, unsigned len, std::string& out )
{

}

void TransCode::GBK_to_Unicode( const char* in, unsigned len, std::wstring& out )
{

}

