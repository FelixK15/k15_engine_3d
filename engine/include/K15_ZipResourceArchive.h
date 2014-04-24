/**
 * @file K15_ZipResourceFile.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/15
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

#ifndef _K15Engine_Core_ZipResourceFile_h_
#define _K15Engine_Core_ZipResourceFile_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ResourceFileBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API ZipResourceFile : public ResourceArchiveBase
	{
	public:
		ZipResourceFile(const String& p_ZipFile);
		~ZipResourceFile();

		virtual bool getResource(const String& p_ResourceName, RawData* p_Data);
		virtual bool hasResource(const String& p_ResourceName);

	protected:
		virtual bool _open();
		virtual bool _close(); 

	private:
		unzFile m_ZipFile;
	};// end of ZipResourceFile class declaration
}}// end of K15_Engine::Core namespace

#endif //