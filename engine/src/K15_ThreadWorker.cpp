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
	K15_IMPLEMENT_RTTI(K15_Engine::Core,ThreadWorkerTask);
	/*********************************************************************************/

	/*********************************************************************************/
	ThreadWorkerTask::ThreadWorkerTask()
		: TaskBase()
	{
		setName(_TN(ThreadWorkerTask));
	}
	/*********************************************************************************/
	ThreadWorkerTask::~ThreadWorkerTask()
	{

	}
	/*********************************************************************************/
	void ThreadWorkerTask::update(const GameTime& p_GameTime)
	{
		g_ThreadWorker->executeJobs();
	}
	/*********************************************************************************/

	/*********************************************************************************/
	void ThreadWorker::execute(JobBase* p_Job)
	{
		p_Job->setStatus(JobBase::JS_RUNNING);
		p_Job->execute();
		p_Job->setStatus(JobBase::JS_FINISHED);
	}
	/*********************************************************************************/
	ThreadWorker::ThreadWorker()
		: m_Jobs(),
		  m_Threads(),
		  m_HardwareThreads(0)
	{
		m_HardwareThreads = Thread::hardware_concurrency() - 1;

		if(m_HardwareThreads == 0)
		{
			_LogWarning("No hardware threading found. Creating 3 default threads.");
			m_HardwareThreads = 3;
		}

		m_Threads.resize(m_HardwareThreads);
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
	void ThreadWorker::addJob(JobBase* p_Job)
	{		
		m_Jobs.push_back(p_Job);
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
	const ThreadWorker::JobList& ThreadWorker::getJobs() const
	{
		return m_Jobs;
	}
	/*********************************************************************************/
	const ThreadWorker::ThreadList& ThreadWorker::getThreads() const
	{
		return m_Threads;
	}
	/*********************************************************************************/
	void ThreadWorker::executeJobs()
	{
		while(m_Jobs.size() > 0)
		{
			bool success = false;
			for(ThreadList::iterator iter = m_Threads.begin();iter != m_Threads.end();++iter)
			{
				if(!(*iter)->joinable())
				{
					success = true;
					(*iter) = new Thread(execute,(*m_Jobs.begin()));
					m_JobThreadMap.insert(Pair(JobBase*,Thread::id)((*m_Jobs.begin()),(*iter)->get_id()));
					m_Jobs.pop_front();
					break;
				}
			}

			if(!success)
			{
				break;
			}
		}

		for(JobThreadMap::iterator iter = m_JobThreadMap.begin();iter != m_JobThreadMap.end();++iter)
		{
			if(iter->first->getStatus() == JobBase::JS_FINISHED)
			{
				_LogDebug("Job %s finished.",iter->first->getName().c_str());
				//find thread
				for(ThreadList::iterator thread_iter = m_Threads.begin();thread_iter != m_Threads.end();++thread_iter)
				{
					if((*thread_iter)->get_id() == iter->second)
					{
						_LogDebug("Joining thread %u.",iter->second.hash());
						(*thread_iter)->join();
						m_JobThreadMap.erase(iter->first);
						return;
					}
				}
			}
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace