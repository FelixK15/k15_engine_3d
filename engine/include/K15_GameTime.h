/**
 * @file K15_GameTime.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/10
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
#ifndef _K15Engine_System_GameTime_h_
#define _K15Engine_System_GameTime_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace System 
{
	class K15_API_EXPORT GameTime
	{
	public:
		GameTime(double p_DeltaTime,double p_Multiplier = 1.0);
		~GameTime();

		/**
		* get the delta time (e.g. the time it took to process the last frame).
		* This is the modified value. (the real delta time multiplied by the multiplier)
		*
		* @return double - modified delta time (unmodified if getMultiplier() == 1.0)
		*/
		inline const double getDeltaTime() const;

		/**
		* get the delta time (e.g. the time it took to process the last frame).
		* This is the normal value. (the real delta time)
		*
		* @return double - unmodified delta time (unmodified if getMultiplier() == 1.0)
		*/
		inline const double getRawDeltaTime() const;

		/**
		* Returns the multiplier that affects the value of getDeltaTime()
		*
		* @return double - multiplier that affects the value of getDeltaTime()
		*/
		inline const double getMultiplier() const;
	private:
		double m_DeltaTime;
		double m_Mulitplicator;
		double m_RawDeltaTime;
	};
	#include "K15_GameTime.inl"
}}// end of K15_Engine::System namespace

#endif //_K15Engine_System_GameTime_h_