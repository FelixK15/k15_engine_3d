/**
 * @file K15_ResourceImporterFont.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/13
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

#ifndef _K15Engine_Core_ResourceImporterFont_h_
#define _K15Engine_Core_ResourceImporterFont_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ResourceImporterBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API ResourceImporterFont : public ResourceImporterBase, public GeneralAllocatedObject
	{
	public:
		ResourceImporterFont();
		~ResourceImporterFont();

		virtual void getExtensionList(ExtensionSet& p_ExtensionSet) OVERRIDE;
		virtual void getMagicNumber(MagicNumberSet& p_MagicNumber) OVERRIDE;

		virtual ResourceBase* _load(const RawData& p_ResourceData, const Rtti& p_ResourceType) OVERRIDE;
	}; //K15_ResourceImporterFont class declaration
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ResourceImporterFont_h_