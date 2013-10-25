/**
 * @file K15_ApplicationOSLayer_Win32.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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

#ifndef _K15Engine_Core_ApplicationOSLayer_Win32_h_
#define _K15Engine_Core_ApplicationOSLayer_Win32_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif// K15_USE_PRECOMPILED_HEADER

#include "K15_ApplicationOSLayerBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	class K15_CORE_API ApplicationOSLayer_Win32 : public ApplicationOSLayerBase
	{
	public:
		ApplicationOSLayer_Win32();
		virtual ~ApplicationOSLayer_Win32();
		// initialize the OS layer
		virtual bool initialize();
		// shutdown OS layer
		virtual void shutdown();
		// get the last error set by the OS
		virtual String getError() const;
		// get supported resolutions
		virtual void getSupportedResolutions(SupportedResolutionSet* p_ResolutionSet) const;
		// get current time (1.0 = 1 sec)
		virtual double getTime() const;
		// idle for x seconds
		virtual void sleep(double p_TimeInSeconds) const;
		// callback before the game gets ticked (pumps messages)
		virtual void onPreTick();
		// callback after the game gets ticked
		virtual void onPostTick();
	private:
		LARGE_INTEGER m_PerformanceCounterFrequency;
		double m_Frequency;
	};//end of ApplicationOSLayer_Win32 class
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ApplicationOSLayer_Win32_h_