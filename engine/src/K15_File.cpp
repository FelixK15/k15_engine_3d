/**
 * @file K15_File.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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
#include "K15_File.h"
#include "K15_LogManager.h"

using namespace K15_EngineV2;

File::File(const char *pFile,U32 iOpenFlags)
	: m_pFile(NULL),
	  m_pFileName(pFile)
{
	Open(pFile,iOpenFlags);
}

File::~File()
{
	if(IsOpen()){
		Close();
	}
}

bool File::Read( U32 iElementSize,U32 iSize,byte *&pBuffer )
{
	if(!IsOpen()){
		WriteDefaultLog(String("Could not read from file because it is closed. Filename: ") + m_pFileName);
		return false;
	}

	pBuffer = K15_NEW byte[iElementSize * iSize];

	U32 iReturn = fread(pBuffer,iElementSize,iSize,m_pFile);

	if(iReturn != iSize){
		WriteDefaultLog(String("Could not read all elements. Filename: ") + m_pFileName);
		
		if(feof(m_pFile) != 0){
			WriteDefaultLog(String("Attempt to read past end of file. Filename: ") + m_pFileName);
		}
	}
	

	return true;
}

bool File::ReadLine( byte *&pBuffer )
{
	if(!IsOpen()){
		WriteDefaultLog(String("Could not read from file because it is closed. Filename: ") + m_pFileName);
		return false;
	}

	pBuffer = K15_NEW byte[256];

	fgets((char*)pBuffer,256,m_pFile);

	return true;
}

bool File::Write( U32 iElementSize,U32 iSize,byte *pBuffer,bool bDeleteBuffer)
{
	assert(pBuffer);
	
	if(!IsOpen()){
		WriteDefaultLog(String("Could not write to file because it is closed. Filename: ") + m_pFileName);
		return false;
	}

	fwrite(pBuffer,iElementSize,iSize,m_pFile);
	fflush(m_pFile);
	rewind(m_pFile);
	if(bDeleteBuffer){
		K15_DELETE[] pBuffer;
	}

	return true;
}

void File::Close()
{
	if(m_pFile){
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

bool File::Open( const char *pFileName,U32 iOpenFlags )
{
	if(m_pFile){
		Close();
	}

	String sMode;
	
	if(HAS_FLAG(File::OM_WRITE,iOpenFlags)){
		sMode += 'w';
	}

	if(HAS_FLAG(File::OM_READ,iOpenFlags)){
		if(sMode == ""){
			sMode += 'r';
		}else{
			sMode += '+';
		}
	}

	if(HAS_FLAG(File::OM_APPEND,iOpenFlags)){
		if(sMode == ""){
			sMode += 'a';
		}else if(sMode.FindLastOf('+') == -1){
			sMode += '+';
		}
	}

	if(HAS_FLAG(File::OM_BINARY,iOpenFlags)){
		sMode += 'b';
	}

	WriteDefaultLog(String("Trying to open file. Filename: ") + pFileName);

	m_pFile = fopen(pFileName,sMode.C_Str());

	if(!m_pFile){
		WriteDefaultLog(String("Could not open file. Filename: ") + m_pFileName);
		WriteDefaultLog(strerror(errno));
		return false;
	}
	
	return m_pFile != NULL;
}

bool File::Exists( const char *pFileName )
{
	FILE *pTemporary = fopen(pFileName,"r+");
	bool bReturn = pTemporary != NULL;

	if(pTemporary){
		fclose(pTemporary);
	}

	return bReturn;
}

bool File::Erase( const char *pFileName )
{
	bool bReturn = remove(pFileName) == 0;
	
	if(!bReturn){
		WriteDefaultLog(String("Could not delete file. Filename: ") + pFileName);
		WriteDefaultLog(strerror(errno));
	}

	return bReturn;
}

bool File::Rename( const char *pFileName,const char *pNewFileName )
{
	bool bReturn = rename(pFileName,pNewFileName) == 0;

	if(!bReturn){
		WriteDefaultLog(String("Could not rename file. Filename: ") + pFileName);
		WriteDefaultLog(strerror(errno));
	}

	return bReturn;
}
