/**
 * @file K15_DebugResourceFile.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#pragma once

#ifndef __K15_DEBUGRESOURCEFILE__
#define __K15_DEBUGRESOURCEFILE__

#include "K15_StdInclude.h"
#include "K15_ResourceFile.h"
#include "K15_String.h"

namespace K15_EngineV2
{
	class DebugResourceFile : public ResourceFile
	{
	public:
		DebugResourceFile();
		virtual ~DebugResourceFile();

		virtual void OpenResourceFile();
		virtual void CloseResourceFile();
		virtual bool GetResource( const String &sFileName,byte *&pData,U32 &iSize,char *&pErrorMessage );
		virtual bool HasResource( const char *pFileName );
		virtual bool IsOpen();
	};
}

#endif //__K15_K15_DEBUGRESOURCEFILE__