/**
 * @file K15_ResourceFile.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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

#ifndef _K15Engine_Core_ResourceFileBase_h_
#define _K15Engine_Core_ResourceFileBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core { 

	class ResourceFileBase
	{
	public:
		virtual bool open() = 0;
		virtual bool close() = 0;

		virtual bool getResource(const String&, ResourceData*) = 0;
		virtual bool hasResource(const String&) = 0;

		virtual String getResourceFileName() = 0;

		virtual bool isOpen() = 0;

	};
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ResourceFileBase_h_