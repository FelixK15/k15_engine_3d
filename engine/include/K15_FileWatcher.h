/**
 * @file K15_FileWatcher.h
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

#ifndef __K15_FILEWATCHER__
#define __K15_FILEWATCHER__

#include "K15_StdInclude.h"
#include "K15_Singleton.h"

#define g_pFileWatcher K15_EngineV2::FileWatcher::GetInstance()

namespace K15_EngineV2
{
	class K15ENGINE2_API FileWatcher : public Singleton<FileWatcher>
	{
	public:
		FileWatcher();
		~FileWatcher();

		bool WatchDirectory(const char *pDirectory);
		void UnwatchDirectory(const char *pDirectory);
		void Update();
	};
}

#endif //__K15_FILEWATCHER__