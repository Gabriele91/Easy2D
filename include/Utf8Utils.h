#pragma once

#ifndef UTF8UTILS_H
#define UTF8UTILS_H

#include <cstdint>
#include <string>
#include <ConvertUTF.h>

namespace Easy2D
{
	typedef char          char8;
	typedef unsigned char uchar8;
	typedef std::int16_t  char16;
	typedef std::uint16_t uchar16;
	typedef std::int32_t  char32;
	typedef std::uint32_t uchar32;
	typedef uchar8        utf8Encoding[4];

	namespace Utf8Utils
	{
		inline void unicodeToUtf8(uchar32 inChar, utf8Encoding& outChar, size_t& outSize, bool ordered = true)
		{
			//swap
			if (!ordered)
				inChar = ((inChar & 0x000000ff) << 24) +
				((inChar & 0x0000ff00) << 8) +
				((inChar & 0x00ff0000) >> 8) +
				((inChar & 0xff000000) >> 24);

			if (inChar < 0x80)
			{
				// 1 byte encoding
				outChar[0] = (char)inChar;
				outSize = 1;
			}
			else if (inChar < 0x800)
			{
				// 2 byte encoding
				outChar[0] = 0xC0 + ((inChar & 0x7C0) >> 6);
				outChar[1] = 0x80 + (inChar & 0x3F);
				outSize = 2;
			}
			else if (inChar < 0x10000)
			{
				// 3 byte encoding
				outChar[0] = 0xE0 + ((inChar & 0xF000) >> 12);
				outChar[1] = 0x80 + ((inChar & 0xFC0) >> 6);
				outChar[2] = 0x80 + (inChar & 0x3F);
				outSize = 3;
			}
			else
			{
				// 4 byte encoding
				outChar[0] = 0xF8 + ((inChar & 0x1C0000) >> 18);
				outChar[1] = 0x80 + ((inChar & 0x3F000) >> 12);
				outChar[2] = 0x80 + ((inChar & 0xFC0) >> 6);
				outChar[3] = 0x80 + (inChar & 0x3F);
				outSize = 4;
			}
		}

		inline void unicodeToUtf8(uchar16 inChar, utf8Encoding& outChar, size_t& outSize, bool ordered = true)
		{
			//swap
			if (!ordered)
				inChar = ((inChar & 0x00ff) << 8) + ((inChar & 0xff00) >> 8);

			//cast to uchar32
			unicodeToUtf8((uchar32)inChar, outChar, outSize, true);
		}

		inline uchar32 utf8ToUnicode(const uchar8 *utf8data)
		{
			if (utf8data[0] < 0x80)
			{
				return (uchar32)utf8data[0];
			}
			else if (utf8data[0] < 0xE0)
			{
				// 2 bytes
				return ((utf8data[0] & 0x1F) << 6) + (utf8data[1] & 0x3F);
			}
			else if (utf8data[0] < 0xF0)
			{
				// 3 bytes
				return ((utf8data[0] & 0xF) << 12) + ((utf8data[1] & 0x3F) << 6) + (utf8data[2] & 0x3F);
			}
			else
			{
				// 4 bytes
				return ((utf8data[0] & 0x7) << 18) + ((utf8data[1] & 0x3F) << 12) + ((utf8data[2] & 0x3F) << 6) + (utf8data[3] & 0x3F);
			}
		}

		inline uchar32 utf16ToUnicode(const uchar16 *utf16data)
		{
			uchar32 ucode;
			assert(0xD800 <= utf16data[0] && utf16data[0] <= 0xDBFF);
			assert(0xDC00 <= utf16data[1] && utf16data[1] <= 0xDFFF);
			ucode = 0x10000;
			ucode += (utf16data[0] & 0x03FF) << 10;
			ucode += (utf16data[1] & 0x03FF);
			return ucode;
		}
		
		inline void nextCharacter(const uchar8 *&utf8data)
		{
			#if 1
				utf8data += trailingBytesForUTF8[*utf8data] + 1;
			#else
			// increments the iterator by one
			if (*utf8data < 0x80)
				++utf8data;
			else if (*utf8data < 0xE0)
				utf8data += 2;		// 2 bytes
			else if (*utf8data < 0xF0)
				utf8data += 3;		// 3 bytes
			else
				utf8data += 4;		// 4 bytes
			#endif

		}

		inline void beforeCharacter(const uchar8 *&utf8data)
		{
			// decrements the iterator by one
			do --utf8data;	while ((*utf8data & 0xc0) == 0x80);
		}

		inline void nextNCharacter(const uchar8 *&utf8data, size_t pos)
		{
			for (size_t i = 0; (*utf8data) && (i != pos); )
			{
				nextCharacter(utf8data);
				++i;
			}
		}

		inline const uchar8* getUCharPtrFromBufferPosition(const uchar8* string, size_t bufferPos)
		{
			//TODO this could not be O(n)?
			const uchar8 *endAddr = &string[bufferPos];
			while (string < endAddr) nextCharacter(string);
			return string;
		}

		inline size_t length(const  uchar8* utf8data)
		{
			size_t n = 0;
			while (*utf8data) { nextCharacter(utf8data); ++n; }
			return n;
		}
		

		inline bool isLegalUTF8String(const uchar8 *source, const uchar8 *sourceEnd)
		{
			while (source < sourceEnd)
			{
				//length of u8char
				int length = trailingBytesForUTF8[*source] + 1;
				//is a correct utf8 sequence?
				if (length > sourceEnd - source || !isLegalUTF8Sequence(source, source+length))
					return false;
				//next char
				(source) += length;
			}
			return true;
		}

		std::wstring utf8ToWString(const uchar8 *source,size_t size);

		std::string wstringToUtf8(const std::wstring& widestring);

		template <typename CharType, typename Alloc>
		inline void makeUTF8StringImp(const CharType* instring,
								      std::basic_string< uchar8, std::char_traits<uchar8>, Alloc> &out,
									  bool appendToOut)
		{
			// first empty the string
			if (!appendToOut) out.clear();

			//LE|BE
			bool defaultOrder = true;
			int start = 0; // index of the first real character in the string

			// check for byte order mark
			if (instring[0] == 0xfffe)
			{
				defaultOrder = false;
				++start;
			}
			else if (instring[0] == 0xfeff)
			{
				// jump past the byte order mark
				++start;
			}

			// loop until null terminator is reached
			for (int i = start; instring[i] != 0; ++i)
			{
				utf8Encoding curEncoding;
				size_t encodingSize;

				// convert to UTF-8
				unicodeToUtf8(instring[i], curEncoding, encodingSize, defaultOrder);

				// add to the std::string
				for (size_t j = 0; j != encodingSize; ++j) out += curEncoding[j];
			}
		}

		template <typename Alloc>
		inline void makeUTF8String(const uchar16* instringUCS2,
			std::basic_string< uchar8, std::char_traits<uchar8>, Alloc> &out,
			bool appendToOut = false)
		{
			// call the template method
			makeUTF8StringImp(instringUCS2, out, appendToOut);
		}

		template <typename Alloc>
		inline void makeUTF8String(const uchar32* instringUCS4,
								   std::basic_string< uchar8, std::char_traits<uchar8>, Alloc > &out,
								   bool appendToOut = false)
		{
			// call the template method
			makeUTF8StringImp(instringUCS4, out, appendToOut);
		}

		template <typename Alloc>
		inline void makeUTF8String(const wchar_t* instringUCS2or4,
								   std::basic_string< uchar8, std::char_traits<uchar8>, Alloc > &out,
								   bool appendToOut = false)
		{
			switch (sizeof(wchar_t))
			{
			case 2:  makeUTF8StringImp((const uchar16*)instringUCS2or4, out, appendToOut); break;
			case 4:  makeUTF8StringImp((const uchar32*)instringUCS2or4, out, appendToOut); break;
			default: /*assert*/ break;
			}
		}
	};
};

#endif