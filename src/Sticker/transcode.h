#ifndef _TRANSCODE_H_
#define _TRANSCODE_H_

#include <string>

class TransCode
{
public:
	static void Unicode_to_UTF8(const wchar_t* in, unsigned len, std::string& out);
	static void UTF8_to_Unicode(const char* in, unsigned len, std::wstring& out);
	static void GBK_to_UTF8(const char* in, unsigned len, std::string& out);
	static void UTF8_to_GBK(const char* in, unsigned len, std::string& out);
	static void Unicode_to_GBK(const wchar_t* in, unsigned len, std::string& out);
	static void GBK_to_Unicode(const char* in, unsigned len, std::wstring& out);

};

#endif