/**
 * @file K15_EventHandler.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/02
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

#ifndef _K15Engine_Core_EventHandler_h_
#define _K15Engine_Core_EventHandler_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#define K15_EVENTHANDLER(c,f,o)(EventHandler::create<c,c::f>(o));

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	struct K15_CORE_API EventArgs
	{
		EventArgs(Object* sender, uint32 type, void* userdata = 0);

		uint32 type;
		Object* sender;
		void* userdata;
	}; //EventArgs struct declaration
	/*********************************************************************************/
	class K15_CORE_API EventHandler
	{
		typedef bool (*HandlerFunction)(void*, const EventArgs&);
	public:
		EventHandler();
		EventHandler(const EventHandler& p_Rhs);

		~EventHandler();

		template<class T, bool (T::*MemberFunction)(const EventArgs&)>
		static EventHandler create(T* p_Object);

		INLINE bool operator()(const EventArgs& p_Args) const;

	private:
		template<class T, bool (T::*MemberFunction)(const EventArgs&)>
		static bool getHandlerFunction(void* p_Object, const EventArgs& p_Args);

	private:
		Object* m_Object;
		HandlerFunction m_Handler;
	}; //EventHandler class declaration
#	include "K15_EventHandler.inl"
	/*********************************************************************************/
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_EventHandler_h_