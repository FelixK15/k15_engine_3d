/**
 * @file K15_GameObjectComponentBase.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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
 */

#ifndef _K15Engine_Core_GameObjectComponent_h_
#define _K15Engine_Core_GameObjectComponent_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Object.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core {
	class K15_CORE_API GameObjectComponentBase : public Object
	{
	/*********************************************************************************/
	K15_DECLARE_RTTI;
	/*********************************************************************************/

	public:
		GameObjectComponentBase(){}
		virtual ~GameObjectComponentBase(){}

		inline GameObject* getGameObject() const;

		virtual void update(const GameTime& p_GameTime){}

	protected:
		GameObject* m_GameObject;
	}; // end of GameObjectComponentBase class declaration
	#include "K15_GameObjectComponentBase.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_GameObjectComponent_h_