#include <stdafx.h>
#include <String.h>
#include <Utf8Utils.h>
#include <ConvertUTF.h>

using namespace Easy2D;
using namespace Utf8Utils;

std::wstring Easy2D::Utf8Utils::utf8ToWString(const uchar8 *source, size_t widesize)
{
	if (sizeof(wchar_t) == 2)
	{
		std::wstring resultstring;
		resultstring.resize(widesize + 1, L'\0');
		const UTF8* sourcestart = reinterpret_cast<const UTF8*>(source);
		const UTF8* sourceend = sourcestart + widesize;
		UTF16* targetstart = reinterpret_cast<UTF16*>(&resultstring[0]);
		UTF16* targetend = targetstart + widesize;
		ConversionResult res = convertUTF8toUTF16(&sourcestart, sourceend, &targetstart, targetend, UTF_STRICT_CONVERSION );
		if (res != UTF_CONVERSION_OK)
		{
			throw std::exception("utf8ToWChar");
		}
		*targetstart = 0;
		return resultstring;
	}
	else if (sizeof(wchar_t) == 4)
	{
		std::wstring resultstring;
		resultstring.resize(widesize + 1, L'\0');
		const UTF8* sourcestart = reinterpret_cast<const UTF8*>(source);
		const UTF8* sourceend = sourcestart + widesize;
		UTF32* targetstart = reinterpret_cast<UTF32*>(&resultstring[0]);
		UTF32* targetend = targetstart + widesize;
		ConversionResult res = convertUTF8toUTF32(&sourcestart, sourceend, &targetstart, targetend, UTF_STRICT_CONVERSION );
		if (res != UTF_CONVERSION_OK)
		{
			throw std::exception("utf8ToWChar");
		}
		*targetstart = 0;
		return resultstring;
	}
	else
	{
		throw std::exception("utf8ToWChar");
	}
	return L"";
}
std::string Easy2D::Utf8Utils::wstringToUtf8(const std::wstring& widestring)
{
	size_t widesize = widestring.length();

	if (sizeof(wchar_t) == 2)
	{
		size_t utf8size = 3 * widesize + 1;
		std::string resultstring;
		resultstring.resize(utf8size, '\0');
		const UTF16* sourcestart = reinterpret_cast<const UTF16*>(widestring.c_str());
		const UTF16* sourceend = sourcestart + widesize;
		UTF8* targetstart = reinterpret_cast<UTF8*>(&resultstring[0]);
		UTF8* targetend = targetstart + utf8size;
		ConversionResult res = convertUTF16toUTF8(&sourcestart, sourceend, &targetstart, targetend, UTF_STRICT_CONVERSION);
		if (res != UTF_CONVERSION_OK)
		{
			throw std::exception("wCharToUtf8");
		}
		*targetstart = 0;
		return resultstring;
	}
	else if (sizeof(wchar_t) == 4)
	{
		size_t utf8size = 4 * widesize + 1;
		std::string resultstring;
		resultstring.resize(utf8size, '\0');
		const UTF32* sourcestart = reinterpret_cast<const UTF32*>(widestring.c_str());
		const UTF32* sourceend = sourcestart + widesize;
		UTF8* targetstart = reinterpret_cast<UTF8*>(&resultstring[0]);
		UTF8* targetend = targetstart + utf8size;
		ConversionResult res = convertUTF32toUTF8(&sourcestart, sourceend, &targetstart, targetend, UTF_STRICT_CONVERSION);
		if (res != UTF_CONVERSION_OK)
		{
			throw std::exception("wCharToUtf8");
		}
		*targetstart = 0;
		return resultstring;
	}
	else
	{
		throw std::exception("wCharToUtf8");
	}
	return "";
}