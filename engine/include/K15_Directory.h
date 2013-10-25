/**
 * @file K15_Directory.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/20
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

#ifndef __K15_DIRECTORY__
#define __K15_DIRECTORY__

#include "K15_StdInclude.h"
#include "K15_List.h"
#include "K15_String.h"

namespace K15_EngineV2
{
	class K15_CORE_API Directory
	{
	public:
		static bool GetFileList(const char *pDirectory,List<String> &lFilesOut,bool bRecursive = true,const char *pFilter = "*.*");
	};
}

#endif //__K15_K15_DIRECTORY__