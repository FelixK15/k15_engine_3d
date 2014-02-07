/**
 * @file K15_MaterialData.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/15
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

#ifndef _K15Engine_Rendering_MaterialData_h_
#define _K15Engine_Rendering_MaterialData_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_ResourceBase.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API MaterialData : public Object
	{
	public:
		/*********************************************************************************/
		K15_DECLARE_RTTI;
		typedef HashMap(String,String) NameValueMap;
		/*********************************************************************************/
	public:
		MaterialData();
		MaterialData(const ObjectName& p_Name);
		~MaterialData();

		bool setValue(const String& p_Name,const String& p_Value);
		INLINE void setMaterialName(const ObjectName& p_MaterialName);

		const String& getValue(const String& p_Name) const;
		
	private:
		NameValueMap m_NameValueMap;
	};// end of MaterialData class declaration
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_MaterialData_h_