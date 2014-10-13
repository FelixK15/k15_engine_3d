/**
 * @file K15_InputManager.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#ifndef _K15Engine_Core_InputManager_h_
#define _K15Engine_Core_InputManager_h_

#include "K15_Prerequisites.h"
#include "K15_Singleton.h"
#include "K15_Application.h"
#include "K15_AllocatedObject.h"
#include "K15_InputTriggerBase.h"

#define K15_InputHandler(c,f,o) K15_Engine::Core::InputHandler::create<c, &c::f>(o)

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	class K15_CORE_API InputEvent
	{
	public:
		InputEvent(Enum p_InputSource, float p_Value);
		~InputEvent();

		INLINE float getValue() const;
		INLINE Enum getInputSource() const;

	private:
		float m_Value;
		Enum m_InputSource;
	}; // end of InputEvent class declaration
	/*********************************************************************************/


	/*********************************************************************************/
	class K15_CORE_API InputHandler
	{
	public:
		typedef bool (*HandlerFunction)(void*, InputEvent*);

	public:
		InputHandler();
		InputHandler(const InputHandler& p_InputHandler);

		~InputHandler();

		template<class T, bool (T::*MemberFunction)(InputEvent*)>
		static InputHandler create(const T* p_Object);

		INLINE bool operator()(InputEvent* p_Arg) const;
		INLINE bool operator==(const InputHandler& p_Rhs) const;

		INLINE bool isValid() const;

	private:
		template<class T, bool (T::*MemberFunction)(InputEvent*)>
		static bool getHandlerFunction(void* p_Object, InputEvent* p_Args);

	private:
		void* m_Object;
		HandlerFunction m_Handler;
	}; // end of InputHandler class declaration
	/*********************************************************************************/



	/*********************************************************************************/
	struct K15_CORE_API InputMapping
	{
		InputHandler Handler;
		ObjectName Device;
		ObjectName Action;
		Enum Key;
	};
	/*********************************************************************************/



	/*********************************************************************************/
	struct TriggeredInput
	{
		ObjectName Device;
		Enum Key;
		float Value;
	};
	/*********************************************************************************/



	/*********************************************************************************/
	class K15_CORE_API InputManager : public CoreAllocatedObject,
									  public Singleton<InputManager>
	{
	public:
		/*********************************************************************************/
		static const uint8 MaxMapCount = 2;
		typedef DynamicArray(InputMapping) InputMappingArray;
		typedef DynamicArray(TriggeredInput) TriggeredInputArray;
		/*********************************************************************************/

	public:
		InputManager();
		virtual ~InputManager();

		bool init();
		void shutdown();

		void parseInputConfig();

		void captureInput();

		void addInputBinding(const ObjectName& p_ActionName, const InputHandler& p_InputHandler);

		void removeInputBinding(const ObjectName& p_ActionName);

		float getValue(const ObjectName& p_ActionName);

	private:
		bool onRenderWindowInitialized(GameEvent* p_Args);

	private:
		InputMappingArray m_InputMapping;
		TriggeredInputArray m_TriggeredInput;
	};
	/*********************************************************************************/
	#include "K15_InputManager.inl"
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_InputManager_h_