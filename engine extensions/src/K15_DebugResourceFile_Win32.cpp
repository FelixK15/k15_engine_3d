/**
 * @file K15_DebugResourceFile.cpp
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

#include "K15_DebugResourceFile.h"
#include "K15_FileManager.h"

using namespace K15_EngineV2;

DebugResourceFile::DebugResourceFile()
{

}

DebugResourceFile::~DebugResourceFile()
{

}

void DebugResourceFile::OpenResourceFile()
{

}

void DebugResourceFile::CloseResourceFile()
{

}

bool DebugResourceFile::GetResource( const String &sFileName,byte *&pData,U32 &iSize,char *&pErrorMessage )
{
	FilePtr pFile = g_pFileManager->FileOpen(sFileName);

	pData = K15_NEW byte[pFile->GetFileSize()];
	iSize = pFile->GetFileSize();

	return g_pFileManager->FileRead(pFile,pFile->GetFileSize(),NULL,pData);
}

bool DebugResourceFile::HasResource( const char *pFileName )
{
	return true;
}

bool DebugResourceFile::IsOpen()
{
	return true;
}