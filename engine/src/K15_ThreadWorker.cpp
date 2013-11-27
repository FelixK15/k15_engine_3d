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
	void ThreadWorker::execute()
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
        jobs.pop_front();
        _LogDebug("Thread %u will process job %s.",g_CurrentThread::get_id().hash(),job->getName().c_str());
      }

      mutex.unlock();

      if(job)
      {
        job->setStatus(JobBase::JS_RUNNING);
        job->execute();
        job->setStatus(JobBase::JS_FINISHED);
      }
    }
	}
	/*********************************************************************************/
	ThreadWorker::ThreadWorker()
		: PoolAllocator<Thread>(ApplicationAllocator,HardwareThreads,_N(ThreadAllocator)),
    m_Jobs(),
		m_Threads()
	{

	}
	/*********************************************************************************/
	ThreadWorker::~ThreadWorker()
	{
		for(ThreadList::iterator iter = m_Threads.begin();iter != m_Threads.end();++iter)
		{
			if((*iter)->joinable())
			{
				(*iter)->join();
			}
		}

		m_Threads.clear();
		m_Jobs.clear();
	}
	/*********************************************************************************/
  void ThreadWorker::initialize()
  {
    Running = true;
    uint8 counter = 0;
    while(counter++ < 0)
    {
      Thread* thread = K15_NEW_T(this,Thread) Thread(execute);
      m_Threads.push_back(thread);
    }
  }
  /*********************************************************************************/
  void ThreadWorker::shutdown()
  {
    Running = false;
    Thread* thread = 0;
    while(m_Threads.size() > 0)
    {
      thread = (*m_Threads.begin());
      thread->join();
      K15_DELETE_T(this,thread);
      m_Threads.pop_front();
    }
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
		m_Jobs.remove(p_Job);
	}
	/*********************************************************************************/
	void ThreadWorker::removeJobByName(const ObjectName& p_Name)
	{
		for(JobList::iterator iter = m_Jobs.begin();iter != m_Jobs.end();++iter)
		{
			if((*iter)->getName() == p_Name)
			{
				m_Jobs.remove((*iter));
			}
		}
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