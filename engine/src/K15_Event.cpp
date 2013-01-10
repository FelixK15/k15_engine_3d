/**
 * @file K15_Event.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/09
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

#include "K15_Event.h"

using namespace K15_EngineV2;

ArgumentMemoryHeap Event::ms_mhArgumentHeap;

Event::Event(EventType etType,void *pArgument,U32 iArgumentSize)
	: m_etType(etType)
{
	m_etType = etType;
	m_iArgumentSize = iArgumentSize;
	
	m_pArgument = ms_mhArgumentHeap.Allocate(m_iArgumentSize);
	Memory::MemCopy(m_pArgument,pArgument,m_iArgumentSize);
}

Event::Event( const Event &evSource )
	: m_etType(evSource.m_etType)
{
	_CopyFrom(evSource);
}

Event::~Event()
{
	if(m_pArgument != NULL){
		ms_mhArgumentHeap.Free(m_pArgument);
		m_pArgument = NULL;
	}
}

const Event& Event::operator=(const Event& evSource)
{
	_CopyFrom(evSource);

	return *this;
}

bool Event::operator==(const Event& evOtherEvent) const
{
	return m_etType.getIdentifier() == evOtherEvent.GetType().getIdentifier();
}

bool Event::operator!=(const Event& evOtherEvent) const
{
	return !(m_etType.getIdentifier() == evOtherEvent.GetType().getIdentifier());
}

void Event::_CopyFrom( const Event& evSource )
{
	m_etType = evSource.m_etType;
	m_iArgumentSize = evSource.m_iArgumentSize;

	if(evSource.m_pArgument){
		m_pArgument = ms_mhArgumentHeap.Allocate(m_iArgumentSize);
		Memory::MemCopy(m_pArgument,evSource.m_pArgument,m_iArgumentSize);
	}
}
