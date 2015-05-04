/**
 * @file K15_EngineTestFirstPersonControls.h
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
 */

#ifndef _K15Engine_TestApplication_FirstPersonControls_h_
#define _K15Engine_TestApplication_FirstPersonControls_h_

#include "K15_GameObjectComponentBase.h"

#include "K15_Quaternion.h"

namespace K15_Engine { namespace Test {
	class FirstPersonControlComponent : public GameObjectComponentBase
	{
	public:
		static ObjectName const InputMoveLeft;
		static ObjectName const InputMoveRight;
		static ObjectName const InputMoveForward;
		static ObjectName const InputMoveBackward;

		static ObjectName const InputLookHorizontal;
		static ObjectName const InputLookVertical;
		static ObjectName const InputReset;

	public:
		FirstPersonControlComponent();
		virtual ~FirstPersonControlComponent();

		virtual void update(const GameTime& p_GameTime);

		bool moveForward(InputEvent* p_Arg);
		bool moveLeft(InputEvent* p_Arg);
		bool moveRight(InputEvent* p_Arg);
		bool moveBackward(InputEvent* p_Arg);

		bool rotateY(InputEvent* p_Arg);
		bool rotateX(InputEvent* p_Arg);
		
// 		INLINE void setSpeed(float p_Speed);
// 		INLINE float getSpeed() const;

	private:
		Quaternion m_Rotation;
		float m_RotationY;
		float m_RotationX;
		bool m_RotationDirty;
	}; // end of FirstPersonControls class declaration
	#include "K15_EngineTestFirstPersonControls.inl"
}} // end of K15_Engine::Test namespace

#endif //_K15Engine_TestApplication_FirstPersonControls_h_