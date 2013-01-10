/**
 * @file K15_Mutex.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/19
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
 * This class is a wrapper of the mutex class from the TinyThread++ library which you can find under:
 * http://tinythreadpp.bitsnbites.eu/
 *
 * The class provides basic mutex support.
 */
#pragma once

#ifndef __K15_MUTEX__
#define __K15_MUTEX__

#include "K15_StdInclude.h"
#include "tinythread.h"

namespace K15_EngineV2
{
	class K15ENGINE2_API Mutex
	{
	public:
		Mutex();
		~Mutex();

		inline void Lock();
		inline void Unlock();
		inline bool TryLock();

	private:
		Mutex(const Mutex &muMutex);
		const Mutex &operator=(const Mutex &muMutex);

	private:
		tthread::mutex m_muMutex;
	};
	#include "..\src\K15_Mutex.inl"
}

#endif //__K15_MUTEX__