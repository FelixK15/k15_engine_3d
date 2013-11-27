/**
 * @file K15_ThreadWorker.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/26
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

#ifndef _K15Engine_Core_ThreadWorker_h_
#define _K15Engine_Core_ThreadWorker_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Singleton.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_Application.h"
#include "K15_PoolAllocator.h"
#include "K15_TaskBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API ThreadWorker : public Singleton<ThreadWorker>, public ApplicationAllocatedObject, public PoolAllocator<Thread>
	{
	public:
		/*********************************************************************************/
		typedef List(JobBase*) JobList;
		typedef List(Thread*) ThreadList;
		typedef HashMap(JobBase*,Thread::id) JobThreadMap;
		static void execute(void*);
    static bool Running;
    static const uint8 HardwareThreads;
    static const uint8 DefaultThreadCount;
		/*********************************************************************************/
	public:
		ThreadWorker();
		virtual ~ThreadWorker();

		void addJob(JobBase* p_Job);

		void removeJob(JobBase* p_Job);
		void removeJobByName(const ObjectName& p_Name);

		JobList& getJobs();
		const ThreadList& getThreads() const;
	private:
		ThreadList m_Threads;
		JobList m_Jobs;
		JobThreadMap m_JobThreadMap;
	};// end of * class declaration
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ThreadWorker_h_