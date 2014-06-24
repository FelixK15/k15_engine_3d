/**
 * @file K15_TaskBase.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 *
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

#ifndef _K15Engine_Core_TaskBase_h_
#define _K15Engine_Core_TaskBase_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_TaskManager.h"
#include "K15_Object.h"
#include "K15_GameTime.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API TaskBase : public Object, public TaskManagerAllocatedObject
	{
	/*********************************************************************************/
	K15_DECLARE_RTTI;
	/*********************************************************************************/

	public:
		TaskBase(uint32 p_Priority = 0);
		~TaskBase();

		virtual void update(const GameTime& p_GameTime) = 0;
		
		INLINE uint32 getPriority() const;
		INLINE void setPriority(uint32 p_Priority);

		INLINE uint32 getFrameModFilter() const;
		INLINE void setFrameModFilter(uint32 p_FrameModFilter);

		bool operator<(const TaskBase& p_Other);

	protected:
		uint32 m_Priority;
		uint32 m_FrameModFilter;
	};// end of TaskBase class
#include "K15_TaskBase.inl"
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_TaskBase_h_