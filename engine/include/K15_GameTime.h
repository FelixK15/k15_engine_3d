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
#ifndef _K15Engine_Core_GameTime_h_
#define _K15Engine_Core_GameTime_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif// K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core {
	class K15_CORE_API GameTime
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
		inline const double getDeltaTime() const{return m_DeltaTime * m_Mulitplicator;}

		/**
		* get the delta time (e.g. the time it took to process the last frame).
		* This is the normal value. (the real delta time)
		*
		* @return double - unmodified delta time (unmodified if getMultiplier() == 1.0)
		*/
		inline const double getRawDeltaTime() const{return m_RawDeltaTime;}

		/**
		* Returns the multiplier that affects the value of getDeltaTime()
		*
		* @return double - multiplier that affects the value of getDeltaTime()
		*/
		inline const double getMultiplier() const{return m_Mulitplicator;}

		inline void setDeltaTime(double p_DeltaTime){m_DeltaTime = p_DeltaTime;}

		inline void setMultiplier(double p_Multiplier){m_Mulitplicator = p_Multiplier;}
	private:
		double m_DeltaTime;
		double m_Mulitplicator;
		double m_RawDeltaTime;
	};// end of GameTime class
	//#include "K15_GameTime.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_GameTime_h_