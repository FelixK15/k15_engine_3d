/**
 * @file K15_Event.h
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
 * 
 */
#pragma once

#ifndef __K15_EVENT__
#define __K15_EVENT__

#include "K15_StdInclude.h"
#include "K15_HashedString.h"
#include "K15_EventTypes.h"

#include "K15_MemoryHeap.h"
//#include "K15_Conversion.h"

namespace K15_EngineV2
{
	typedef HashedString EventType;
	typedef MemoryHeap<byte,MEGABYTE> ArgumentMemoryHeap;

	class K15ENGINE2_API Event
	{
	public:
		Event(EventType etType,void *pArgument,size_t iArgumentSize);
		Event(const Event &evSource);
		~Event();

		const EventType GetType() const;
		const size_t GetArgumentSize() const;

		void *GetArgument() const;

		const Event &operator=(const Event& evSource);

		bool operator==(const Event& evOtherEvent) const;
		bool operator!=(const Event& evOtherEvent) const;

	private:
		void _CopyFrom(const Event& evSource);

	private:
		static ArgumentMemoryHeap ms_mhArgumentHeap;

		EventType m_etType;
		size_t m_iArgumentSize;
		void *m_pArgument;
	};

	#include "../src/K15_Event.inl"
}

#endif //__K15_EVENT__