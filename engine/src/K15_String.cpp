/**
 * @file K15_String.cpp
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
 */

#include "K15_String.h"
#include "K15_Object.h"
#include "K15_System.h"
#include "K15_CStdIncludes.h"

#include "wchar.h"


using namespace K15_EngineV2;

String::String(unsigned int iSize,unsigned int iReallocationSize)
{
	_InitString(iSize,iReallocationSize);
}

String::String(const K15_CHAR* sString,unsigned int iSize,unsigned int iReallocationSize)
{
	assert(sString);

	_InitString(iSize,iReallocationSize);
	_Append(sString,STR_LEN(sString));
}

String::String(const String& sString,unsigned int iReallocationSize)
{
	_InitString(sString.Size(),iReallocationSize);
	_Append(sString.C_Str(),sString.Size());
}

String::String(String&& sString)
{
	if(sString.m_pStr != 0){
		this->m_pStr = sString.m_pStr;
		this->m_iSizeStr = sString.m_iSizeStr;
		this->m_iCapacity = sString.m_iCapacity;
		this->m_iReallocationSize = sString.m_iReallocationSize;

		sString.m_pStr = 0;
	}else{
		_InitString(32,32);
	}
}

String::~String()
{
	if(m_pStr != 0){
		K15_DELETE[] m_pStr;
	}
}

const K15_CHAR* String::C_Str() const
{
	return m_pStr;
}

const unsigned int String::Size() const
{
	return m_iSizeStr;
}

const unsigned int String::Capacity() const
{
	return m_iCapacity;
}

const unsigned int String::ReallocationSize() const
{
	return m_iReallocationSize;
}

const int String::FindFirstOf( const K15_CHAR sCharacter ) const
{
	unsigned int iSize = Size();
	int iPos = -1;

	for(unsigned int i = 0;i < iSize;++i){
		if(m_pStr[i] == sCharacter){
			iPos = i;
			break;
		}
	}

	return iPos;
}

const int String::FindLastOf( const K15_CHAR sCharacter ) const
{
	int iPos = -1;

	for(unsigned int i = Size();i <= 0;--i){
		if(m_pStr[i] == sCharacter){
			iPos = i;
			break;
		}
	}

	return iPos;
}

String String::SubString( unsigned int iStartPos )
{
	return SubString(iStartPos,0xffffff);
}

String String::SubString( unsigned int iStartPos,unsigned int iEndPos )
{
	assert(iStartPos < m_iSizeStr);
	
	uint32 iSize = m_iSizeStr - iStartPos;

	if(iEndPos != 0xffffff){
		assert(iEndPos <= m_iSizeStr);
		iSize = iEndPos - iSize;
	}

	const char *pBuffer = K15_NEW char[iSize];
	Memory::MemCopy((void*)pBuffer,&m_pStr[iStartPos],iSize);

	String sSubString = pBuffer;
	K15_DELETE[] pBuffer;

	return sSubString; 
}

bool String::IsEmpty()
{
	return m_iSizeStr == 0;
}

void String::Clear()
{
	m_iSizeStr = 0;
	m_pStr[0] = '/0';
}

const String& String::Insert( uint32 iPos,const String& sString )
{
	assert(iPos < Size());

	if(iPos == Size()){
		_Append(sString.C_Str(),sString.Size());
	}

	unsigned int iStringSize = sString.Size();
	if(sString[sString.Size()] == '/0'){
		--iStringSize;
	}

	unsigned int iNewSize = Size() + iStringSize;
	
	if(iNewSize > Capacity()){
		unsigned int iTempReallocationSize = m_iReallocationSize;
		m_iReallocationSize = Capacity() - iNewSize;
		_Realloc();
		m_iReallocationSize = iTempReallocationSize;
	}

	Memory::MemMove(m_pStr + (iPos + sString.Size()),m_pStr + iPos,Size() - iPos);
	Memory::MemCopy(m_pStr + iPos,sString.C_Str(),sString.Size());

	m_iSizeStr = iNewSize;

	return *this;
}

const String& String::Replace(uint32 iPos,const String& sString)
{
	assert(iPos < Size());

	unsigned int iNewSize = iPos + sString.Size();

	if(iNewSize > Capacity()){
		unsigned int iTempReallocationSize = m_iReallocationSize;
		m_iReallocationSize = Capacity() - iNewSize;
		_Realloc();
		m_iReallocationSize = iTempReallocationSize;
	}

	if(iNewSize > Size()){
		m_iSizeStr = iPos;
		_Append(sString.C_Str(),sString.Size());
	}else{
		Memory::MemCopy(m_pStr + iPos,sString.C_Str(),sString.Size());
		m_iSizeStr = iPos + sString.Size();
	}

	return *this;
}

/*bool String::Contains( const CHAR* str,unsigned int *pos,unsigned int flag ) const
{

}

bool String::Contains( const String& str,unsigned int *pos,unsigned int flag ) const
{

}*/

const K15_CHAR String::operator[]( const unsigned int iPos ) const
{
	if(iPos > Size()){
		return '@'; //place holder
	}

	return m_pStr[iPos];
}

String& String::operator=( const String &sString )
{
	_DeleteArray();
	_Append(sString.m_pStr,sString.m_iSizeStr);

	return *this;
}

String& String::operator=(String &&sString)
{
	if(sString.m_pStr != 0){
		_DeleteArray();

		this->m_pStr = sString.m_pStr;
		this->m_iSizeStr = sString.m_iSizeStr;
		this->m_iReallocationSize = sString.m_iReallocationSize;

		sString.m_pStr = 0;
	}

	return *this;
}

String& String::operator=(const K15_CHAR *sString)
{
	assert(sString);

	_DeleteArray();
	_Append(sString,STR_LEN(sString));

	return *this;
}

bool String::operator==( const K15_CHAR* sString ) const
{
	return _Compare(sString);
}

bool String::operator==( const String &sString ) const
{
	return _Compare(sString.C_Str());
}

bool String::operator!=( const K15_CHAR* sString ) const
{
	return !_Compare(sString);
}

bool String::operator!=( const String &sString ) const
{
	return !_Compare(sString.C_Str());
}

bool String::operator>( const K15_CHAR* sString) const
{
	return _GreaterThan(sString);
}

bool String::operator>( const String& sString) const
{
	return _GreaterThan(sString.C_Str());
}

bool String::operator<( const K15_CHAR* sString) const
{
	return !_GreaterThan(sString);
}

bool String::operator<( const String& sString) const
{
	return !_GreaterThan(sString.C_Str());
}

const String& String::operator+=( const K15_CHAR sCharacter )
{
	_Append(&sCharacter,1);
	return *this;
}

const String& String::operator+=( const String &sString )
{
	_Append(sString.C_Str(),sString.Size());
	return *this;
}

const String& String::operator+=( const K15_CHAR* sString )
{
	assert(sString);

	if(sString != 0){
		_Append(sString,STR_LEN(sString));
	}

	return *this;
}

const String& String::operator+=( Object *pObject )
{
	_AddObject(pObject);

	return *this;
}

const String& String::operator+( const K15_CHAR sCharacter )
{
	_Append(&sCharacter,1);

	return *this;
}

const String& String::operator+( const String &sString )
{
	_Append(sString.C_Str(),sString.Size());

	return *this;
}

const String& String::operator+( const K15_CHAR* sString )
{
	assert(sString);

	_Append(sString,STR_LEN(sString));

	return *this;
}

const String& String::operator+( Object *pObject )
{
	_AddObject(pObject);
	return *this;
}

void String::_InitString( unsigned int iCapacity,unsigned int iReallocationSize )
{
	m_iSizeStr = 0;
	m_iCapacity = iCapacity;
	m_iReallocationSize = iReallocationSize;

	m_pStr = K15_NEW K15_CHAR[iCapacity];
}

void String::_AddObject( Object *pObject )
{
//	String pObjectString = pObject->ToString();
//	_Append(pObjectString.C_Str(),pObjectString.Size());
}

bool String::_Compare( const K15_CHAR* sString ) const
{
	assert(sString);

	unsigned int iSizeStr = Size();
	unsigned int iSizeStr2 = STR_LEN(sString);

	if(iSizeStr != iSizeStr2){
		return false;
	}

	return STR_CMP(m_pStr,sString) == 0;
}

/*bool String::_Contains( const CHAR* str,U32 *pos ) const
{

}

bool String::_RContains( const CHAR* str,U32 *pos ) const
{

}*/

void String::_DeleteArray()
{
	if(m_pStr){
		K15_DELETE[] m_pStr;
		m_pStr = 0;
	}
}

void String::_Append( const K15_CHAR* sString,unsigned int iSize )
{
	assert(sString != 0 && iSize > 0);

	//Has sString a null terminator?
	bool bNullTerminator = _HasNullTerminator(sString,iSize);

	//If the string has no null terminator, increase the size.
	if(!bNullTerminator){
		++iSize;
	}

	unsigned int iSizeNeeded = (Size() + iSize) + 1; // +1 for Null terminated string

	if(iSizeNeeded >= Capacity()){
		unsigned int iOldReallocationSize = m_iReallocationSize;
		m_iReallocationSize = (iSizeNeeded - Capacity());
		_Realloc();
		m_iReallocationSize = iOldReallocationSize;
	}

	if(!m_pStr){
		_InitString(Capacity(),ReallocationSize());
	}

	Memory::MemCopy(m_pStr + Size(),sString,sizeof(K15_CHAR) * iSize);

	m_iSizeStr += iSize;
	m_pStr[m_iSizeStr] = '\0';
}

void String::_Realloc()
{
	unsigned int iNewSize = Capacity() + m_iReallocationSize;

	K15_CHAR* pTempString = K15_NEW K15_CHAR[iNewSize];

	if(m_pStr){
		Memory::MemCopy(pTempString,m_pStr,Size());
		K15_DELETE[] m_pStr;
	}
	
	m_pStr = pTempString;

	m_iCapacity = iNewSize;
}

bool String::_GreaterThan( const K15_CHAR* sString ) const
{
	size_t iSizeStr = STR_LEN(sString);
	size_t iSizeThis = Size();

	for(size_t i = 0;i < iSizeStr || i < iSizeThis;++i){
		if(tolower(sString[i]) < tolower(m_pStr[i])){
			return true;
		}
	}

	return false;
}

bool String::_HasNullTerminator( const K15_CHAR* sString,unsigned int iSize )
{
	return sString[iSize] == '\0';
}