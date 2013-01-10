/**
 * @file K15_Thread.h
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
 * This is a wrapper for the thread class from the TinyThread++ library, which you can find under:
 * http://tinythreadpp.bitsnbites.eu/
 *
 * The class provides basic thread support.
 */

#pragma once
#ifndef __K15_THREAD__
#define __K15_THREAD__

#include "K15_StdInclude.h"

namespace tthread{
	class thread;
}

namespace K15_EngineV2
{
	typedef void(*ThreadFunction)(void*);

	class K15ENGINE2_API Thread
	{
	public:
		/**
		* Constructor of the Thread class.
		*
		* @param  pFunction Pointer to a function of type void FUNCTION(void*). This function will get executed by this thread.
		* @param  pParameter Pointer to parameters that will passed to the pFunction parameter.
		*/
		Thread(ThreadFunction pFunction,void *pParameter);
		~Thread();

		/**
		* Detaches a thread from its parent so the thread "runs on its own". The thread is not joinable after detaching.
		*/
		void Detach();

		/**
		* The calling thread waits until the thread finished its task.
		*/
		void Join();

		/**
		* Check if the thread is joinable.
		* @return bool true if thread is joinable, false if not.
		*/
		bool Joinable();

	private:
		Thread(const Thread &tThread);
		const Thread &operator=(const Thread &tThread);

	private:
		tthread::thread *m_tThread;
	};
}

#endif //K15_THREAD 