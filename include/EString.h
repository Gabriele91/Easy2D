#pragma once

#ifndef ESTRING_H
#define ESTRING_H

#include <Config.h>
#include <cstring>
#include <string>
#include <vector>
#include <cstdint>
#include <cwctype>
#include <Utf8Utils.h>

namespace Easy2D
{
	template < class Alloc = std::allocator< uchar8 > >
	class UTFString : public Pointers < UTFString< Alloc > >
	{
    public:
        
		//types
		typedef std::uint32_t              ValueType;
		typedef std::uint32_t*             Pointer;
		typedef std::uint32_t&             Reference;
		typedef size_t					   SizeType;
		typedef ptrdiff_t                  DifferenceType;

		typedef const std::uint32_t*       ConstPointer;
		typedef const std::uint32_t&       ConstReference;

    protected:
        
		//raw data
		std::basic_string< uchar8, std::char_traits< uchar8 >, Alloc> mRawData;
		
		template <class T>
		class TIterator : public std::iterator<std::bidirectional_iterator_tag, T >
		{
		private:
			const uchar8* utf8strBuf;

			void inc()
			{
				// increments the iterator by one
				Utf8Utils::nextCharacter(utf8strBuf);
			}

			void dec()
			{
				// decrements the iterator by one
				Utf8Utils::beforeCharacter(utf8strBuf);
			}

		public:

			TIterator() : utf8strBuf(nullptr) {}
			// b should be a null terminated string in UTF-8
			// if this is the end start_pos should be the index of the null terminator
			// start_pos should be the valid start of a character
			TIterator(const uchar8 *b, SizeType startPos)
			:utf8strBuf(b)
			{
				Utf8Utils::nextNCharacter(utf8strBuf, startPos);
			}

			// copy constructor
			TIterator(const TIterator &other)
			:utf8strBuf(other.utf8strBuf)
			{
			}

			// b should already point to the correct position in the string
			explicit TIterator(const uchar8 *b)
			:utf8strBuf(b)
			{
			}

			ValueType operator*() const
			{
				// returns the character currently being pointed to
				return Utf8Utils::utf8ToUnicode(utf8strBuf);
			}

			// does not check to see if it goes past the end
			// iterating past the end is undefined
			TIterator& operator++()
			{
				inc();

				return *this;
			}

			// does not check to see if it goes past the end
			// iterating past the end is undefined
			TIterator operator++(int)
			{
				TIterator copy(*this);

				inc();

				return copy;
			}

			// does not check to see if it goes past the end
			// iterating past begin is undefined
			TIterator &operator--()
			{
				dec();
				return *this;
			}

			// does not check to see if it goes past the end
			// iterating past begin is undefined
			TIterator operator--(int)
			{
				TIterator copy(*this);

				dec();

				return copy;
			}

			TIterator operator + (SizeType n) const
			{
				TIterator copy = *this;
				for (SizeType i = 0; i != n; ++i, copy.inc());
				return copy;
			}

			TIterator operator - (SizeType n) const
			{
				auto copy = *this;
				for (SizeType i = 0; i != n; ++i, copy.dec());
				return copy;
			}
			
			bool operator == (const TIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return utf8strBuf == other.utf8strBuf;
			}

			bool operator != (const TIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return utf8strBuf != other.utf8strBuf;
			}

			bool operator < (const TIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return utf8strBuf < other.utf8strBuf;
			}

			bool operator > (const TIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return utf8strBuf > other.utf8strBuf;
			}

			bool operator <= (const TIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return utf8strBuf <= other.utf8strBuf;
			}

			bool operator >= (const TIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return utf8strBuf >= other.utf8strBuf;
			}

			//raw ptr
			const uchar8* getPtr() const
			{
				return utf8strBuf;
			}
		};

		template <class TBaseIterator>
		class TReverseIterator : public std::iterator<std::bidirectional_iterator_tag, ValueType>
		{
		public:
			TBaseIterator forwardIt;

		public:
			// constructor
			TReverseIterator(){}

			// copy constructor
			TReverseIterator(const TReverseIterator &other)
			:forwardIt(other.forwardIt)
			{
			}

			// create from forward iterator
			TReverseIterator(const TBaseIterator &iterator)
			:forwardIt(iterator)
			{

			}

			ValueType operator*() const
			{
				TBaseIterator temp = forwardIt;
				return *(--temp);
			}

			// does not check to see if it goes past the end
			// iterating past the end is undefined
			TReverseIterator &operator++()
			{
				--forwardIt;

				return *this;
			}

			// does not check to see if it goes past the end
			// iterating past the end is undefined
			TReverseIterator operator++(int)
			{
				TReverseIterator copy(*this);

				// increment
				--forwardIt;

				return copy;
			}

			// does not check to see if it goes past the end
			// iterating past begin is undefined
			TReverseIterator &operator--()
			{
				++forwardIt;
				return *this;
			}

			// does not check to see if it goes past the end
			// iterating past begin is undefined
			TReverseIterator operator--(int)
			{
				TReverseIterator copy(*this);

				++forwardIt;

				return copy;
			}

			TReverseIterator operator + (SizeType n) const
			{
				TReverseIterator copy = *this;
				for (SizeType i = 0; i != n; ++i, ++copy);
				return copy;
			}

			TReverseIterator operator - (SizeType n) const
			{
				TReverseIterator copy = *this;
				for (SizeType i = 0; i != n; ++i, --copy);
				return copy;
			}

			bool operator == (const TReverseIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return forwardIt == other.forwardIt;
			}

			bool operator != (const TReverseIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return forwardIt != other.forwardIt;
			}

			bool operator < (const TReverseIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return forwardIt > other.forwardIt;
			}

			bool operator > (const TReverseIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return forwardIt < other.forwardIt;
			}

			bool operator <= (const TReverseIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return forwardIt >= other.forwardIt;
			}

			bool operator >= (const TReverseIterator &other) const
			{
				// just compare pointers
				// the programmer is responsible for making both iterators are for the same set of data
				return forwardIt <= other.forwardIt;
			}
			
			//cast op
			explicit operator TBaseIterator (void) const
			{
				return forwardIt;
			}

			//raw ptr
			const uchar8* getPtr() const
			{
				return forwardIt.getPtr();
			}
		};

	public:
		//iterators
		typedef TIterator < uchar8 >               Iterator;
		typedef TIterator < const uchar8 >         ConstIterator;
		typedef TReverseIterator < Iterator >      ReverseIterator;
		typedef TReverseIterator < ConstIterator > ConstReverseIterator;
		static const SizeType npos = std::numeric_limits<SizeType>::max();
		//void string
		static const UTFString NONE;

		//constructors
		UTFString()
		{
		}
		UTFString(const UTFString& u8str)
		: mRawData(u8str.mRawData)
		{
		}
		UTFString(const uchar8* u8str)
		: mRawData(u8str)
		{
		}		
		UTFString(const char8* u8str)
		: mRawData((const uchar8*)u8str)
		{
		}
		UTFString(const char8 *str, size_t n)
		: mRawData((const uchar8*)str, n)
		{
		}
		UTFString(const uchar8* u8str, size_t n)
		:mRawData(u8str, n)
		{
		}
		UTFString(const std::string& u8str)
		: mRawData((const uchar8*)u8str.c_str())
		{
		}
		UTFString(const std::wstring& ustr)
		{
			std::string u8str = Utf8Utils::wstringToUtf8(ustr);
			mRawData = (const uchar8*)u8str.c_str();
		}
		UTFString(const std::u16string& ustr)
		{
			Utf8Utils::makeUTF8String(ustr.c_str(), mRawData, true);
		}
		UTFString(const std::u32string& ustr)
		{
			Utf8Utils::makeUTF8String(ustr.c_str(), mRawData, true);
		}
		UTFString(uchar8 c)
		{
			mRawData.resize(1);
			mRawData[0] = c;
		}
		UTFString(char8 c)
		{
			mRawData.resize(1);
			mRawData[0] = (uchar8)c;
		}
		UTFString(uchar32 c)
		{
			//ucs4 char to utf8
			utf8Encoding encoding;
			size_t encodingSize;
			Utf8Utils::unicodeToUtf8(c, encoding, encodingSize);
			//alloc
			mRawData.reserve(encodingSize);
			//copy
			for (size_t i = 0; i != encodingSize; ++i)
			{
				mRawData += encoding[i];
			}
		}		
		UTFString(uchar16 c)
		{
			//ucs4 char to utf8
			utf8Encoding encoding;
			size_t encodingSize;
			Utf8Utils::unicodeToUtf8(c, encoding, encodingSize);
			//alloc
			mRawData.reserve(encodingSize);
			//copy
			for (size_t i = 0; i != encodingSize; ++i)
			{
				mRawData += encoding[i];
			}
		}		
		UTFString(wchar_t c)
		:UTFString((uchar32)c)
		{
		}
		UTFString(size_t n, uchar8 c)
		:mRawData(n,c)
		{

		}		
		UTFString(size_t n, char8 c)
		:mRawData(n, (uchar8)c)
		{

		}
		UTFString(size_t n, uchar32 c)
		{
			//ucs4 char to utf8
			utf8Encoding encoding;
			size_t encodingSize;
			Utf8Utils::unicodeToUtf8(c, encoding, encodingSize);
			//alloc
			mRawData.reserve(encodingSize * n);
			//copy
			for (size_t j = 0; j != n; ++j)
			for (size_t i = 0; i != encodingSize; ++i)
			{
				mRawData += encoding[i];
			}
		}
		UTFString(size_t n, uchar16 c)
		{
			//ucs4 char to utf8
			utf8Encoding encoding;
			size_t encodingSize;
			Utf8Utils::unicodeToUtf8(c, encoding, encodingSize);
			//alloc
			mRawData.reserve(encodingSize * n);
			//copy
			for (size_t j = 0; j != n; ++j)
			for (size_t i = 0; i != encodingSize; ++i)
			{
				mRawData += encoding[i];
			}
		}
		UTFString(size_t n, wchar_t c)
		:UTFString(n,(uchar32)c)
		{
		}
		// destructor
		~UTFString()
		{
		}

		// request a new buffer size
		void reserve(SizeType new_size)
		{
			mRawData.reserve(new_size);
		}

		// resizes the length of the string, padding the string with c
		// if the size is greater than the current size
		void resize(SizeType n, ValueType c)
		{
			if (n < size())
			{
				auto end = cbegin() + n;
				// set the null terminator
				mRawData.resize(getBytePosition(end));
			}
			else if (n > size())
			{
				SizeType diff = n - size();

				// convert c to UTF-8 to get the size
				utf8Encoding cUtf8;
				SizeType cRealSize;

				Utf8Utils::unicodeToUtf8(c, cUtf8, cRealSize);

				// total size to add
				SizeType additionalSpace = cRealSize * diff;
				SizeType curSize = mRawData.length();

				// make sure the buffer is big enough
				// we must do this now because if an exception is thrown because of
				// memory, the value of the string shouldn't change
				mRawData.resize(additionalSpace + curSize);

				for (SizeType i = curSize; i < (additionalSpace + curSize); i += cRealSize)
				{
					for (SizeType j = 0; j != cRealSize; ++j) mRawData[i + j] = cUtf8[j];
				}
			}
		}

		//get length
		SizeType size() const
		{
			return Utf8Utils::length(mRawData.c_str());
		}
		
		//get length
		SizeType length() const
		{
			return size();
		}

		// returns the size of the allocated buffer in bytes
		SizeType capacity() const
		{
			return mRawData.capacity();
		}

		// get length in bytes
		SizeType byteSize() const
		{
			return mRawData.size();
		}

		// clears the string, setting the size to 0
		void clear()
		{
			mRawData.clear();
		}

		//buffer dealloc
		void shrinkToit()
		{
			mRawData.shrink_to_fit();
		}

		// checks to see if the string is empty
		bool empty() const
		{
			return mRawData.empty();
		}

		// checks to see if the string isn't empty
		bool notEmpty() const
		{
			return !empty();
		}

		//get std allocator
		Alloc getAllocator() const
		{
			return mRawData.get_allocator();
		}

		//return true if is a legal utf8 string
		bool isLegalUtf8() const
		{
			if (!mRawData.c_str()) return true;
			return Utf8Utils::isLegalUTF8String(data(), data() + byteSize() + 1);
		}

		//get C string
		const char* cStr() const
		{
			return (const char*)mRawData.c_str();
		}

		//raw data
		const uchar8* data() const
		{
			return mRawData.data();
		}
		
		// appends a character to the string
		void pushBack(ValueType c)
		{
			// just use resize to do the work
			resize(length() + 1, c);
		}

		// deletes the last character
		void popBack()
		{
			// just use resize to do the work
			resize(length() - 1);
		}

        //return byte position of a iterator
        size_t getBytePosition(ConstIterator pos) const
        {
            return pos.getPtr() - mRawData.data();
        }
        
        size_t getBytePosition(ConstReverseIterator pos) const
        {
            return pos.getPtr() - mRawData.data();
        }

		//stap string
		void swap(UTFString& str)
		{
			std::swap(str, *this);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////

		// return Iterator to the beginning of the list
		Iterator begin()
		{
			return Iterator(mRawData.c_str());
		}

		// return ConstIterator to the beginning of the list
		ConstIterator cbegin() const
		{
			return ConstIterator(mRawData.c_str());
		}

		// return ConstIterator to the beginning of the list
		ConstIterator begin() const
		{
			return cbegin();
		}

		// return Iterator to just after the last element of the list
		Iterator end()
		{
			return Iterator(mRawData.c_str() + mRawData.length());
		}

		// return ConstIterator to just after the last element of the list
		ConstIterator cend() const
		{
			const auto *endPtr = mRawData.c_str() + mRawData.length();

			return ConstIterator(endPtr);
		}

		// return ConstIterator to just after the last element of the list
		ConstIterator end() const
		{
			return cend();
		}

		// return iterator to the beginning of the list
		ReverseIterator rbegin()
		{
			return ReverseIterator(end());
		}

		// return const_iterator to the beginning of the list
		ConstReverseIterator crbegin() const
		{
			return ConstReverseIterator(end());
		}

		// return const_iterator to the beginning of the list
		ConstReverseIterator rbegin() const
		{
			return crbegin();
		}

		// return iterator to just after the last element of the list
		ReverseIterator rend()
		{
			return ReverseIterator(begin());
		}

		// return iterator to just after the last element of the list
		ConstReverseIterator crend() const
		{
			return ConstReverseIterator(begin());
		}

		// return iterator to just after the last element of the list
		ConstReverseIterator rend() const
		{
			return crend();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// get at UCS4 -------------------------------------------------------------------------------------

		ValueType operator[] (size_t n) 
		{
			const uchar8* tmpStr = data();
			Utf8Utils::nextNCharacter(tmpStr, n);
			return Utf8Utils::utf8ToUnicode(tmpStr);
		}

		ValueType operator[] (size_t n) const
		{
			const uchar8* tmpStr = data();
			Utf8Utils::nextNCharacter(tmpStr, n);
			return Utf8Utils::utf8ToUnicode(tmpStr);
		}

		ValueType at(size_t n)
		{
			const uchar8* tmpStr = data();
			Utf8Utils::nextNCharacter(tmpStr, n);
			return Utf8Utils::utf8ToUnicode(tmpStr);
		}

		ValueType at(size_t n) const
		{
			const uchar8* tmpStr = data();
			Utf8Utils::nextNCharacter(tmpStr, n);
			return Utf8Utils::utf8ToUnicode(tmpStr);
		}

		// modifiers -------------------------------------------------------------------------------------
		
		UTFString& operator= (const UTFString& rvalue)
		{
			mRawData = rvalue.mRawData;
			return *this;
		}

		UTFString& operator= (const UTFString&& rvalue)
		{
			mRawData = std::move(rvalue.mRawData);
			return *this;
		}

        UTFString& operator+= (const UTFString& str)
        {
            //operator from std::basic_string
            mRawData += str.mRawData;
            return *this;
        }
        
        UTFString& operator+= (const char* cstr)
        {
            UTFString string(cstr);
            //operator from std::basic_string
            mRawData += string.mRawData;
            return *this;
        }
        
        UTFString& operator+= (const std::string& cppstr)
        {
            UTFString string(cppstr);
            //operator from std::basic_string
            mRawData += string.mRawData;
            return *this;
        }
        
        UTFString& operator+= (const std::wstring& wcppstr)
        {
            UTFString string(wcppstr);
            //operator from std::basic_string
            mRawData += string.mRawData;
            return *this;
        }

		UTFString& operator+= (uchar8 achar)
		{
			//operator from std::basic_string
			mRawData += achar;
			return *this;
		}

		UTFString& operator+=(char8 achar)
		{
			//operator from std::basic_string
			mRawData += (uchar8)achar;
			return *this;
		}

		UTFString& operator+=(ValueType achar)
		{
			//operator from std::basic_string
			mRawData += UTFString(achar);
			return *this;
		}

		UTFString& operator+=(wchar_t achar)
		{
			//operator from std::basic_string
			mRawData += UTFString(achar);
			return *this;
		}

		UTFString& assign(const char8 *str)
		{
			mRawData.assign(str);
			return *this;
		}

		UTFString& assign(const uchar8 *str)
		{
			mRawData.assign(str);
			return *this;
		}

		UTFString& assign(const UTFString& str)
		{
			mRawData.assign(str.mRawData);
			return *this;
		}

		UTFString& assign(const UTFString&& str)
		{
			mRawData = std::move(str.mRawData);
			return *this;
		}

		UTFString& assign(const std::string& instring)
		{
			mRawData.assign(instring);
			return *this;
		}

		template <class InputIterator>
		UTFString& assign(InputIterator first, InputIterator last)
		{
			UTFString temp;
			for (auto it = first; it < last; it++)
			{
				temp += (ValueType)*it;
			}
			assign(temp);
		}

		// comparison operators ---------------------------------------------------------------------------
		bool operator == (const UTFString& other) const
		{
			return mRawData == other.mRawData;
		}

		bool operator != (const UTFString& other) const
		{
			return mRawData != other.mRawData;
		}

		bool operator < (const UTFString& other) const
		{
			return mRawData < other.mRawData;
		}

		bool operator > (const UTFString& other) const
		{
			return mRawData > other.mRawData;
		}

		bool operator <= (const UTFString& other) const
		{
			return mRawData <= other.mRawData;
		}

		bool operator >= (const UTFString& other) const
		{
			return mRawData >= other.mRawData;
		}

		// string operations -----------------------------------------------------------------------------
        
        UTFString substr(ConstIterator start, ConstIterator end) const
        {
            return UTFString(start.getPtr(), getBytePosition(end) - getBytePosition(start));
        }
        
        UTFString substr(ConstReverseIterator start, ConstReverseIterator end) const
        {
            return UTFString(start.getPtr(), getBytePosition(end) - getBytePosition(start));
        }

		void split(const UTFString& delimiters, std::vector<UTFString>& tokens) const
		{
			// Skip delimiters at beginning.
			ConstIterator lastPos = findFirstNotOf(delimiters);
			// Find first "non-delimiter".
			ConstIterator pos = findFirstOf(delimiters, lastPos);
			//end it
			ConstIterator endIt = end();
			//search all tokens
			while (endIt != pos || endIt != lastPos)
			{
				UTFString token=substr(lastPos, pos);
				// Found a token, add it to the vector.
				tokens.push_back(token);
				// Skip delimiters.  Note the "not_of"
				lastPos = findFirstNotOf(delimiters, pos);
				// Find next "non-delimiter"
				pos = findFirstOf(delimiters, lastPos);
			}
		}

		std::vector<UTFString> split(const UTFString& delimiters) const
		{
			std::vector<UTFString> vouput;
			split(delimiters, vouput);
			return std::move(vouput);
		}

		UTFString trim() const
		{
			//iteratos
			ConstIterator itl;
			ConstReverseIterator itr;
			//search start string
			for (itl = begin(); itl != end(); ++itl)
			{
				if (!std::iswspace(*itl)) break;
			}
			//search end string
			for (itr = rbegin(); itr != rend(); ++itr)
			{
				if (!std::iswspace(*itr)) break;
			}
			//return
			return substr(itl, (ConstIterator)itr);
		}

		UTFString toLower() const
		{
			UTFString ouput;
			//to lower all chars
			for (ConstIterator it = begin(); it != end(); ++it)
			{
				ouput += UTFString((wchar_t)std::towlower(*it));
			}
			//return
			return std::move(ouput);
		}

		UTFString toUpper() const
		{
			UTFString ouput;
			//to upper all chars
			for (ConstIterator it = begin(); it != end(); ++it)
			{
				ouput += UTFString((wchar_t)std::towupper(*it));
			}
			//return
			return std::move(ouput);
		}

		void replace(const UTFString& toReplace, const UTFString& replaceWith)
		{
			//search
			ConstIterator itfind = find(toReplace);
			//find?
			if (itfind != cend())
			{
				size_t bytefind = itfind.getPtr() - mRawData.data();
				mRawData.replace(bytefind, toReplace.size(), replaceWith.mRawData);
			}
		}

		//replaceALL
		void replaceAll(const UTFString& toReplace, const UTFString& replaceWith)
		{
			//search
			ConstIterator itfind = find(toReplace);
			//find?
			while (itfind != cend())
			{
				size_t bytefind = itfind.getPtr() - mRawData.data();
				mRawData.replace(bytefind, toReplace.size(), replaceWith.mRawData);
				//re-search
                itfind = find(toReplace);
			}
		}

		// find operators ---------------------------------------------------------------------------
		ConstIterator find(const UTFString& str, ConstIterator pos = {}) const
		{
			//TODO rewrite using std::string find, avoid linear search in GetCharPtrFromBufferPosition
			SizeType realPos = getBytePosition(pos.getPtr() ? pos : begin());
			SizeType foundPos = mRawData.find(str.mRawData, realPos);
			
			// return the character position
			return foundPos == std::string::npos ?
				   end() :
				   ConstIterator(Utf8Utils::getUCharPtrFromBufferPosition(mRawData.c_str(), foundPos));
		}

		ConstIterator rfind(const UTFString& str, ConstIterator pos = {}) const
		{
			//TODO rewrite using std::string find, avoid linear search in GetCharPtrFromBufferPosition
			SizeType realPos = getBytePosition(pos.getPtr() ? pos : end());
			SizeType foundPos = mRawData.rfind(str.mRawData, realPos);

			// return the character position
			return foundPos == std::string::npos ?
				   end() :
				   ConstIterator(Utf8Utils::getUCharPtrFromBufferPosition(mRawData.c_str(), foundPos));
		}

		ConstIterator findFirstOf(const UTFString& str, ConstIterator pos = {}) const
		{
			SizeType realPos = getBytePosition(pos.getPtr() ? pos : begin());
			SizeType foundPos = mRawData.find_first_of(str.mRawData, realPos);

			// return the character position
			return foundPos == std::string::npos ?
				   end() :
				   ConstIterator(Utf8Utils::getUCharPtrFromBufferPosition(mRawData.c_str(), foundPos));
		}

		ConstIterator findLastOf(const UTFString& str, ConstIterator pos = {}) const
		{
			if (pos == ConstIterator{}) pos = str.end();
			SizeType foundPos = mRawData.find_last_of(str.mRawData, getBytePosition(pos));

			// return the character position
			return foundPos == std::string::npos ?
				   end() :
				   ConstIterator(Utf8Utils::getUCharPtrFromBufferPosition(mRawData.c_str(), foundPos));
		}

		ConstIterator findFirstNotOf(const UTFString& str, ConstIterator pos = {}) const
		{

			SizeType realPos = getBytePosition(pos.getPtr() ? pos : begin());
			SizeType foundPos = mRawData.find_first_not_of(str.mRawData, realPos);

			// return the character position
			return  foundPos == std::string::npos ?
					end() :
					ConstIterator(Utf8Utils::getUCharPtrFromBufferPosition(mRawData.c_str(), foundPos));
		}

		// cast operators ---------------------------------------------------------------------------

		//UTF16 or UTF32 cast
		operator std::wstring() const
		{
			return Utf8Utils::utf8ToWString(data(),byteSize());
		}

		//std::string cast
		operator std::string () const
		{
			//return byte string
			return (const char*)mRawData.c_str();
		}
		
		//c string cast
		operator const char*() const
		{
			//return byte string
			return (const char*)mRawData.c_str();
		}

		template<typename T>
		static UTFString toString(T t)
		{
            std::wstring wstr(std::to_wstring(t));
			return UTFString(wstr);
		}
        
        static bool isSpace(UTFString::ValueType c)
        {
            return std::iswspace((wchar_t)c);
        }
	};

	//static string NONE
	template <class Alloc> 
	UTFString<Alloc> const UTFString<Alloc>::NONE("");

	//default string
	typedef Easy2D::UTFString< std::allocator< uchar8 > > String;

	// overload stream insertion so we can write to streams
	template <class Alloc>
	std::ostream& operator<<(std::ostream& os, const UTFString<Alloc>& string)
	{
		os << (const char *)string.cStr();
		return os;
	}

	// overload stream extraction so we can write to streams
	template <class Alloc>
	std::istream& operator>>(std::istream& is, UTFString<Alloc>& string)
	{
		std::string in;
		is >> in;
		string = in;
		return is;
	}

	// overload stream insertion so we can write to streams
	template <class Alloc>
	std::wostream& operator<<(std::wostream& os, const UTFString<Alloc>& string)
	{
		os << (std::wstring)string;
		return os;
	}

	// overload stream extraction so we can write to streams
	template <class Alloc>
	std::wistream& operator>>(std::wistream& is, UTFString<Alloc>& string)
	{
		std::wstring in;
		is >> in;
		string = in;
		return is;
	}

	// overload * 
	template<class Alloc, typename N >
	UTFString<Alloc> operator*(const UTFString<Alloc>& s, N n)
	{
		UTFString<Alloc> out;
		for (N i = 0; i != n; ++i) out += s;
		return out;
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (const UTFString<Alloc>& ls, const UTFString<Alloc>& rs)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += rs;
	}
    
    // add op
    template<class Alloc>
    UTFString<Alloc> operator + (const UTFString<Alloc>& ls, const char* rs)
    {
        UTFString<Alloc> utfstr(ls);
        return utfstr += rs;
    }
    
    // add op
    template<class Alloc>
    UTFString<Alloc> operator + (const char* ls, const UTFString<Alloc>& rs)
    {
        UTFString<Alloc> utfstr(ls);
        return utfstr += rs;
    }
    // add op
    template<class Alloc>
    UTFString<Alloc> operator + (const UTFString<Alloc>& ls, char* rs)
    {
        UTFString<Alloc> utfstr(ls);
        return utfstr += rs;
    }
    
    // add op
    template<class Alloc>
    UTFString<Alloc> operator + (char* ls, const UTFString<Alloc>& rs)
    {
        UTFString<Alloc> utfstr(ls);
        return utfstr += rs;
    }

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (const UTFString<Alloc>& ls, const std::string& rs)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += rs;
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (const std::string& ls, const UTFString<Alloc>& rs)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += rs;
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (const UTFString<Alloc>& ls, const std::wstring& rs)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += rs;
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (const UTFString<Alloc>& ls, char rs)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += UTFString<Alloc>(rs);
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (char rs, const UTFString<Alloc>& ls)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += UTFString<Alloc>(rs);
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (const UTFString<Alloc>& ls, wchar_t rs)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += UTFString<Alloc>(rs);
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (wchar_t rs, const UTFString<Alloc>& ls)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += UTFString<Alloc>(rs);
	}

	// add op
	template<class Alloc>
	UTFString<Alloc> operator + (const std::wstring& ls, const UTFString<Alloc>& rs)
	{
		UTFString<Alloc> utfstr(ls);
		return utfstr += rs;
	}

	template<class Alloc, typename N >
	UTFString<Alloc> operator + (const UTFString<Alloc>& s, N n)
	{
		std::wstring wstr(std::to_wstring(n));
		return s + wstr;
	}

	template<class Alloc, typename N >
	UTFString<Alloc> operator + (N n, const UTFString<Alloc>& s)
	{
		std::wstring wstr(std::to_wstring(n));
		return wstr + s;
	}

	//compare equals
	template<class Alloc>
	bool operator == (const UTFString<Alloc>& u8str, const std::wstring& stdstr)
	{
		return u8str == UTFString<Alloc>(stdstr);
	}

	template<class Alloc>
	bool operator == (const std::wstring& stdstr, const UTFString<Alloc>& u8str)
	{
		return UTFString<Alloc>(stdstr) == u8str;
	}

	template<class Alloc>
	bool operator == (const UTFString<Alloc>& u8str, const std::string& stdstr)
	{
		return u8str == UTFString<Alloc>(stdstr);
	}

	template<class Alloc>
	bool operator == (const std::string& stdstr, const UTFString<Alloc>& u8str)
	{
		return UTFString<Alloc>(stdstr) == u8str;
	}

	template<class Alloc>
	bool operator == (const UTFString<Alloc>& u8str, const char* stdstr)
	{
		return u8str == UTFString<Alloc>(stdstr);
	}

	template<class Alloc>
	bool operator == (const char* stdstr, const UTFString<Alloc>& u8str)
	{
		return UTFString<Alloc>(stdstr) == u8str;
	}

	//compare not equals
	template<class Alloc>
	bool operator != (const UTFString<Alloc>& u8str, const std::wstring& stdstr)
	{
		return u8str != UTFString<Alloc>(stdstr);
	}

	template<class Alloc>
	bool operator != (const std::wstring& stdstr, const UTFString<Alloc>& u8str)
	{
		return UTFString<Alloc>(stdstr) != u8str;
	}

	template<class Alloc>
	bool operator != (const UTFString<Alloc>& u8str, const std::string& stdstr)
	{
		return u8str != UTFString<Alloc>(stdstr);
	}

	template<class Alloc>
	bool operator != (const std::string& stdstr, const UTFString<Alloc>& u8str)
	{
		return UTFString<Alloc>(stdstr) != u8str;
	}

	template<class Alloc>
	bool operator != (const UTFString<Alloc>& u8str, const char* stdstr)
	{
		return u8str != UTFString<Alloc>(stdstr);
	}

	template<class Alloc>
	bool operator != (const char* stdstr, const UTFString<Alloc>& u8str)
	{
		return UTFString<Alloc>(stdstr) != u8str;
	}
};

namespace std
{
	template<>
	struct hash< Easy2D::String >
	{
		inline static size_t hashCStr(const char* str)
		{
		   unsigned long h = 0;
		   for (; *str; ++str) h = 5 * h + *str;
		   return size_t(h);
		}

	public:
		size_t operator()(const Easy2D::String& val) const
		{
			return hashCStr(val.cStr());
		}
		size_t operator()(const Easy2D::String* val) const
		{
			return hashCStr(val->cStr());
		}
	};
};

#endif