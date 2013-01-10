/**
 * @file K15_ResourceFile.h
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
#ifndef __K15_RESOURCEFILE__
#define __K15_RESOURCEFILE__

#include "K15_StdInclude.h"
#include "K15_String.h"

namespace K15_EngineV2
{
	class Resource;

	class ResourceFile
	{
	public:
		virtual void OpenResourceFile() = 0;
		virtual void CloseResourceFile() = 0;

		virtual bool GetResource(const String &sFileName,byte *&pData,U32 &iSize,char *&pErrorMessage) = 0;
		virtual bool HasResource(const char*) = 0;

		virtual bool IsOpen() = 0;

	};
}

#endif //__K15_RESOURCEFILE__