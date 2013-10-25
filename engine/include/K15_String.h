/**
 * @file K15_String.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * 
 */
#pragma once

#ifndef __K15_STRING__
#define __K15_STRING__

#include "K15_StdInclude.h"

#ifdef _UNICODE
	#define K15_CHAR wchar_t

	#define STR_CPY(dest,src) wcscpy(dest,src)
	#define STR_CAT(dest,size,src) wcscat_s(dest,size,src)
	#define STR_LEN(str) wcslen(str)
	#define STR_CHR(str,chr) wcschr(str,chr)
	#define STR_RCHR(str,chr) wcsrchr(str,chr)
	#define STR_CMP(str1,str2) wcscmp(str1,str2)
	#define STR_STR(str,substr) wcsstr(str,substr)
#else
	#define K15_CHAR char

	#define STR_CPY(dest,src) strcpy(dest,src)
	#define STR_CAT(dest,size,src) strcat_s(dest,size,src)
	#define STR_LEN(str) strlen(str)
	#define STR_CHR(str,chr) strchr(str,chr)
	#define STR_RCHR(str,chr) wcsrchr(str,chr)
	#define STR_CMP(str1,str2) strcmp(str1,str2)
	#define STR_STR(str,substr) strstr(str,substr)
#endif

namespace K15_Engine { namespace Core { 
	class Object;

	class K15_CORE_API String
	{
	public:
		String(unsigned int iSize = 32,unsigned int iReallocationSize = 32);
		String(const K15_CHAR* sString,unsigned int iSize = 32,unsigned int iReallocationSize = 32);
		String(const String& sSource,unsigned int iReallocationSize = 32);
		String(String&& sSource);

		~String();

		/**
		 * Convert the string to a const K15_CHAR* string.
		 *
		 * @return const K15_CHAR* - Content of the string as const K15_CHAR
		 */
		const K15_CHAR* C_Str() const;

		/**
		 * Returns the size of the string.
		 *
		 * @return const unsigned int - Size of the string.
		 */
		const unsigned int Size() const;

		/**
		 * Returns the capacity of the string (if the size == capactiy, the internal array will get reallocated)
		 *
		 * @return const unsigned int - Capacity of the string.
		 */
		const unsigned int Capacity() const;

		/**
		 * Returns the reallocation size (aka. the size the internal array will grow if it is full)
		 * 
		 * @return const unsigned int - Reallocation size of the string.
		 */
		const unsigned int ReallocationSize() const;

		/**
		 * Functions returns the left most position of a specific char. (or -1 if the char was not found)
		 *
		 * @param sCharacter - Char that you want to know the left most position of.
		 * 
		 * @return const int - left most position of a specific char. (or -1 if the char was not found)
		 */
		const int FindFirstOf(const K15_CHAR sCharacter) const;

		/**
		 * Functions returns the right most position of a specific char. (or -1 if the char was not found)
		 *
		 * @param sCharacter - Char that you want to know the right most position of.
		 * 
		 * @return const int - right most position of a specific char. (or -1 if the char was not found)
		 */
		const int FindLastOf(const K15_CHAR sCharacter) const;

		/**
		 * Creates a new string of the content of this string.
		 *
		 * @param iStartPos - start position of where to start to create a sub string.
		 * 
		 * @return String - new string with the content of a portion of this string (iStartPos to End of String)
		 */
		String SubString(unsigned int iStartPos);
		/**
		 * Creates a new string of the content of this string.
		 *
		 * @param iStartPos - start position of where to start to create a sub string.
		 * @param iEndPos - end position of where to end to create a sub string.
		 * 
		 * @return String - new string with the content of a portion of this string (iStartPos to iEndPos)
		 */
		String SubString(unsigned int iStartPos,unsigned int iEndPos);

		/**
		 * Check whether or not a String object is empty or not.
		 */
		bool IsEmpty();

		/**
		 * Clears the String.
		 */
		void Clear();

		/**
		 * Inserts a given string at a specific position within this string.
		 *
		 * @param iPos - Position at where to start to insert the string.
		 * @param sString - String that will get inserted into this string.
		 * 
		 * @return const String& - New String.
		 */
		const String& Insert(unsigned int iPos,const String& sString);

		/**
		 * Starts to replace this string with another string starting at a specific position.
		 *
		 * @param iPos - Position at where to start to replacing the string.
		 * @param sString - String that will get inserted into this string.
		 * 
		 * @return const String& - New String.
		 */
		const String& Replace(unsigned int iPos,const String& sString);

		/*bool Contains(const CHAR* str,unsigned int *pos,unsigned int flag) const;
		bool Contains(const String& str,unsigned int *pos,unsigned int flag) const;*/

		const K15_CHAR operator[](const unsigned int iPos) const;

		String& operator=(const String &sSource);
		String& operator=(String &&sSource);
		String& operator=(const K15_CHAR *sSource);

		bool operator==(const K15_CHAR* sString) const;
		bool operator==(const String &sString) const;

		bool operator!=(const K15_CHAR* sString) const;
		bool operator!=(const String &sString) const;

		bool operator<(const K15_CHAR *sString) const;
		bool operator<(const String &sString) const;

		bool operator>(const K15_CHAR *sString) const;
		bool operator>(const String &sString) const;

		const String& operator+=(const K15_CHAR sCharacter);
		const String& operator+=(const String &sString);
		const String& operator+=(const K15_CHAR* sString);
		const String& operator+=(Object *pObject);

		const String& operator+(const K15_CHAR sCharacter);
		const String& operator+(const String &sString);
		const String& operator+(const K15_CHAR* sString);
		const String& operator+(Object *pObject);

	private:
		void _InitString(unsigned int iSize,unsigned int iReallocationSize);
		void _AddObject(Object *pObject);

		bool _Compare(const K15_CHAR* sString) const;
		bool _GreaterThan(const K15_CHAR* sString) const;

		/*bool _Contains(const CHAR* str,unsigned int *pos) const;
		bool _RContains(const CHAR* str,unsigned int *pos) const;*/

		void _DeleteArray();

		void _Append(const K15_CHAR* sString,unsigned int iSize);

		void _Realloc();

		bool _HasNullTerminator(const K15_CHAR* sString,unsigned int iSize);

	private:
		K15_CHAR *m_pStr;

		unsigned int m_iReallocationSize;
		unsigned int m_iSizeStr;
		unsigned int m_iCapacity;
	};
}}//end of K15_Engine::Core namespace

#endif //__K15_STRING__