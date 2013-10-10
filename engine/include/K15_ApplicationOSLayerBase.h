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

#ifndef _K15Engine_System_ApplicationOSLayerBase_h_
#define _K15Engine_System_ApplicationOSLayerBase_h_

#include "K15_Prerequisites.h"
#include "K15_Application.h"
#include "K15_AllocatedObject.h"
#include "K15_RenderWindowBase.h"

namespace K15_Engine { namespace System {
	/*********************************************************************************/
	class K15_API_EXPORT ApplicationOSLayerBase : public ApplicationAllocatedObject
	{
	public:
		/*********************************************************************************/
		static const String OSName;
		typedef K15_List(RenderWindowBase::Resolution) SupportedResolutionSet;
		/*********************************************************************************/
	public:
		ApplicationOSLayerBase();
		virtual ~ApplicationOSLayerBase();
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
	};//end of ApplicationOSLayerBase class
	/*********************************************************************************/
}}// end of K15_Engine::System namespace

#endif //_K15Engine_System_ApplicationOSLayerBase_h_