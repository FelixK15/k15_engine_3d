/**
 * @file K15_Thread.cpp
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
 */

#include "K15_Thread.h"
#include "tinythread.h"

using namespace K15_EngineV2;

Thread::Thread(ThreadFunction pFunction,void *pParameter)
	: m_tThread(K15_NEW tthread::thread(pFunction,pParameter))
{
	
}

Thread::~Thread()
{
	if(m_tThread->joinable()){
		m_tThread->join();
	}
}

void Thread::Detach()
{
	m_tThread->detach();
}

void Thread::Join()
{
	m_tThread->join();
}

bool Thread::Joinable()
{
	return m_tThread->joinable();
}

Thread::Thread( const Thread &tThread )
{
	//Not implemented
}

const Thread &Thread::operator=( const Thread &tThread )
{
	//Not implemented
	return *this;
}
