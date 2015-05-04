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
#ifndef _K15Engine_Core_Event_h_
#define _K15Engine_Core_Event_h_

#include "K15_Prerequisites.h"
#include "K15_EventManager.h"

namespace K15_Engine { namespace Core { 
    /*********************************************************************************/
    struct K15_CORE_API ResizeEventArguments
    {
        uint32 oldWidth;   //<! Old resolution (width)
        uint32 oldHeight;  //<! Old resolution (height)

        uint32 newWidth;    //<! New resolution (width)
        uint32 newHeight;   //<! New resolution (height)
    };
    /*********************************************************************************/
    struct K15_CORE_API KeyboardEventArguments
    {
        Enum key;       //<! Check InputDevices::Keyboard::eKey
		uint32 text;
        bool pressed;   //<! Is the key pressed or not?
    };
	/*********************************************************************************/
    struct K15_CORE_API MouseEventArguments
	{
        Enum button;    //<! Check InputDevices::Mouse::eButton

        uint32 xPx;     //<! Pixel coordinates of the mouse cursor
        uint32 yPx;     //<! Pixel coordinates of the mouse cursor

        float wheelDelta;   //<! Mousewheel delta value

        float xNDC;     //<! Normal Device Coordinates ([-1|+1]) of the mouse cursor
        float yNDC;     //<! Normal Device Coordinates ([-1|+1]) of the mouse cursor

        bool pressed;   //<! Is the button pressed or not?
	};
	/*********************************************************************************/
	class K15_CORE_API GameEvent : public GameEventAllocatedObject
	{
	public:
		/**
		* Event constructor.
		*
		* The constructor will copy p_Argument into an internal buffer. 
		*
		* @param  p_Name - Unique name of the event
		* @param  p_Argument - Argument for this event
		* @param  p_ArgumentSize - Size of the argument
		*/
		GameEvent(const EventName& p_Name,void* p_Argument = 0,uint32 p_ArgumentSize = 0);

		/**
		* Event constructor. (copy constructor)
		*
		* The constructor will copy the arguments of the p_Other event.
		*
		* @param  p_Other - Event instance to copy data from
		*/
		GameEvent(const GameEvent &p_Other);

		/**
		* Default destructor
		*
		* Free's the argument memory (if any has been used)
		*/
		~GameEvent();

		/**
		* Get the unique name of this event
		*
		* @return const EventName& - unique name of this event
		*/
		INLINE const EventName& getName() const;

		/**
		* Returns the size of the argument of this event
		*
		* @return const uint32 - size of the argument
		*/
		INLINE const uint32 getArgumentSize() const;

		/**
		* Get the argument of this event.
		* @note the argument will be a raw void pointer, you have to cast it yourself.
		*
		* @return void* - raw void pointer to argument
		*/
		INLINE void* getArgument() const;

		/**
		* Get the argument of this event.(Template)
		*
		* @param  p_Offset - an offset from the start of the memory from the argument
		*
		* @return T - Argument Type
		*/
// 		template<class T>
// 		T getArgument(uint32 p_Offset = 0);

		const GameEvent& operator=(const GameEvent& p_Other);

		bool operator==(const GameEvent& p_Other) const;
		bool operator!=(const GameEvent& p_Other) const;
	private:
		void copyFrom(const GameEvent& p_Other);

	private:
		EventName m_Name;
		uint32 m_ArgumentSize;
		void* m_Argument;
	};
	#include "K15_GameEvent.inl"
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Event_h_