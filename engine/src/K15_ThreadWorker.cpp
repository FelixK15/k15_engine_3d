/**
 * @file K15_ThreadWorker.cpp
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
 */

#include "K15_PrecompiledHeader.h"

#include "K15_ThreadWorker.h"
#include "K15_JobBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	bool ThreadWorker::Running = false;
	const uint8 ThreadWorker::DefaultThreadCount = 3;
	const uint8 ThreadWorker::HardwareThreads = Thread::hardware_concurrency() == 0 ? DefaultThreadCount : Thread::hardware_concurrency() - 1;
	/*********************************************************************************/
	void ThreadWorker::execute(void*)
	{
		static Mutex mutex;
		while(Running)
		{
			JobBase* job = 0;
			mutex.lock();

			JobList& jobs = g_ThreadWorker->getJobs();
			if(jobs.size() > 0)
			{
				job = (*jobs.begin());

				K15_ASSERT(job,"NULL job in job list.");

				jobs.pop_front();
				_LogDebug("Thread %u will process job \"%s\".",0,job->getName().c_str());
			}

			mutex.unlock();

			if(job)
			{
				job->start();
				job->setStatus(JobBase::JS_RUNNING);
				job->execute();
				job->setStatus(JobBase::JS_FINISHED);
				job->end();

				if(job->getAutoDelete())
				{
					K15_DELETE job;
				}
			}

			//put the current thread to sleep for 10 milliseconds.
			OSLayer::sleep(0.01f);
		}
	}
	/*********************************************************************************/
	ThreadWorker::ThreadWorker()
		: m_Jobs(),
		m_Threads()
	{
		Running = true;
		uint8 counter = 0;

		while(counter++ < HardwareThreads)
		{
			_LogNormal("Creating %i/%i thread...",counter,HardwareThreads);
			Thread* thread = K15_NEW_T(Allocators[AC_THREADING],Thread) Thread(execute,(void*)0);
			_LogNormal("Successfully created %i/%i thread",counter,HardwareThreads);

			m_Threads.push_back(thread);
		}
	}
	/*********************************************************************************/
	ThreadWorker::~ThreadWorker()
	{
		Running = false;
		Thread* thread = 0;
		JobBase* job = 0;
		while(m_Threads.size() > 0)
		{
			thread = m_Threads.back();

			K15_ASSERT(thread,"NULL thread in ThreadWorker list.");

			thread->join();
			thread->~thread();
			K15_DELETE_SIZE(Allocators[AC_THREADING],thread,sizeof(Thread));
			m_Threads.pop_back();
		}

		while(m_Jobs.size() > 0)
		{
			job = (*m_Jobs.begin());

			K15_ASSERT(job,"NULL job in job list.");

			if(job->getAutoDelete())
			{
				K15_DELETE job;
			}

			m_Jobs.pop_front();
		}

		m_Threads.clear();
		m_Jobs.clear();
	}
  /*********************************************************************************/
	void ThreadWorker::addJob(JobBase* p_Job)
	{		
		m_Jobs.push_back(p_Job);
		p_Job->setStatus(JobBase::JS_QUEUED);
	}
	/*********************************************************************************/
	void ThreadWorker::removeJob(JobBase* p_Job)
	{
		std::remove(m_Jobs.begin(),m_Jobs.end(),p_Job);
	}
	/*********************************************************************************/
	void ThreadWorker::removeJobByName(const ObjectName& p_Name)
	{
		std::remove_if(m_Jobs.begin(),m_Jobs.end(),
			[&](const JobBase* p_Job)->bool
			{
				return p_Job->getName() == p_Name;
			});
	}
	/*********************************************************************************/
	ThreadWorker::JobList& ThreadWorker::getJobs()
	{
		return m_Jobs;
	}
	/*********************************************************************************/
	const ThreadWorker::ThreadList& ThreadWorker::getThreads() const
	{
		return m_Threads;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace