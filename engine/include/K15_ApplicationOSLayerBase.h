/**
 * @file K15_ApplicationOSLayerBase.h
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

#ifndef _K15Engine_Core_ApplicationOSLayerBase_h_
#define _K15Engine_Core_ApplicationOSLayerBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Application.h"
#	include "K15_AllocatedObject.h"
#endif// K15_USE_PRECOMPILED_HEADER

#include "K15_RenderWindowBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	class K15_CORE_API ApplicationOSLayerBase : public ApplicationAllocatedObject
	{
	public:
		/*********************************************************************************/
		static const String OSName;
		static const String PluginExtension;
		typedef List(Resolution) SupportedResolutionSet;
		/*********************************************************************************/
	public:
		ApplicationOSLayerBase(){}
		virtual ~ApplicationOSLayerBase(){}
		// initialize the OS layer
		virtual bool initialize() = 0;
		// shutdown OS layer
		virtual void shutdown() = 0;
		// get the last error set by the OS
		virtual String getError() const = 0;
		// get supported resolutions
		virtual void getSupportedResolutions(SupportedResolutionSet* p_ResolutionSet) const = 0;
		// get current time
		virtual double getTime() const = 0;
		// idle for x seconds
		virtual void sleep(double p_TimeInSeconds) const = 0;
		// callback before the game gets ticked
		virtual void onPreTick() = 0;
		// callback after the game got ticked
		virtual void onPostTick() = 0;
	};//end of ApplicationOSLayerBase class
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ApplicationOSLayerBase_h_