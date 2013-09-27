/**
 * @file K15_ApplicationModuleDescription.h
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

#ifndef _K15Engine_System_ApplicationModuleDescription_h_
#define _K15Engine_System_ApplicationModuleDescription_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace System { 

	struct ApplicationModuleDescription
	{
		enum PluginFlags
		{
			PF_OwnTask		= 0x002, //does the plugin needs to get updated every frame?
			PF_OwnWindow	= 0x004, //does the plugin needs its own window?
			PF_IsRenderer	= 0x008	 //is the plugin a renderer?
		};

		String Author;
		uint32 MajorVersion;
		uint32 MinorVersion;
		uint32 SettingsBitMask;
	};
}}//end of K15_Engine::System namespace

#endif //_K15Engine_System_ApplicationModuleDescription_h_