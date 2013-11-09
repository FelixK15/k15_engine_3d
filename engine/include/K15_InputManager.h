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

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Singleton.h"
#	include "K15_Application.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_InputTriggerBase.h"
#include "K15_BlockAllocator.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API InputManager : public Singleton<InputManager>, public ApplicationAllocatedObject, public BlockAllocator
	{
	public:
		/*********************************************************************************/
		static const uint8 MaxMapCount = 2;
		typedef HashMap(ObjectName,InputTriggerBase*) InputMapping;
		/*********************************************************************************/

		/*********************************************************************************/
		enum eControllerButton
		{
			BUTTON_A,
			BUTTON_B,
			BUTTON_X,
			BUTTON_Y,

			BUTTON_START,
			BUTTON_BACK,

			BUTTON_LEFT_TRIGGER,
			BUTTON_RIGHT_TRIGGER,

			BUTTON_LEFT_STICK,
			BUTTON_RIGHT_STICK,

			BUTTON_DPAD_UP,
			BUTTON_DPAD_DOWN,
			BUTTON_DPAD_LEFT,
			BUTTON_DPAD_RIGHT
		};//ControllerButton
		/*********************************************************************************/
	public:
		InputManager();
		virtual ~InputManager();

		void addInputBinding(const ObjectName& p_ActonName,InputTriggerBase* p_Trigger);
		//void addInputBinding(const ObjectName& p_ActonName,InputTriggerBase** p_Triggers,uint8 p_TriggerCount);

		void removeInputBinding(const ObjectName& p_ActionName);
		
		bool isActive(const ObjectName& p_ActionName);
	private:
		InputMapping m_InputMap;
	};
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_InputManager_h_