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

#ifndef _K15Engine_Core_ApplicationModuleDescription_h_
#define _K15Engine_Core_ApplicationModuleDescription_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif// K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core { 

	struct ApplicationModuleDescription
	{
		enum PluginFlags
		{
			PF_ProvidesTask     = 0x00000001,
			PF_ProvidesRenderer = 0x00000002
		};

		StringSet GroupFilter;
		String Author;
		uint32 MajorVersion;
		uint32 MinorVersion;
		uint32 PluginFlagBitMask;
		uint32 CompiledWithEngineVersion;
	};
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ApplicationModuleDescription_h_