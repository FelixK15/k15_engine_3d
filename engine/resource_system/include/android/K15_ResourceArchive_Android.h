/**
 * @file K15_ResourceArchiveAndroid.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/09
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

#ifndef _K15Engine_Core_ResourceArchiveAndroid_h_
#define _K15Engine_Core_ResourceArchiveAndroid_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_ResourceArchiveBase.h"

#ifdef K15_OS_ANDROID

namespace K15_Engine { namespace Core {
	class K15_CORE_API ResourceArchiveAndroid : public ResourceArchiveBase, public ResourceAllocatedObject
	{
		K15_DECLARE_RTTI;

	public:
		ResourceArchiveAndroid();
		~ResourceArchiveAndroid();

		virtual bool getResource(const String& p_ResourceName, RawData* p_ResourceData) OVERRIDE;
		virtual bool hasResource(const String& p_ResourceName) OVERRIDE;

		virtual bool _open() OVERRIDE;
		virtual bool _close() OVERRIDE;

	private:
		AAssetManager* m_AssetManager;
	};// end of * class declaration
}}// end of K15_Engine::Core namespace

#endif //K15_OS_ANDROID

#endif //_K15Engine_Core_ResourceArchiveAndroid_h_