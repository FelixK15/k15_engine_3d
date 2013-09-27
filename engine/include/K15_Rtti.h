/**
 * @file K15_Rtti.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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
#pragma once

#ifndef __K15Engine_System_RTTI_h_
#define __K15Engine_System_RTTI_h_

#include "K15_Prerequisites.h"

#include "K15_Rtti.mcr"

namespace K15_Engine { namespace System { 

	class K15_API_EXPORT Rtti
	{
	public:
		Rtti(const String &sName,const Rtti *pBaseType);
		~Rtti();

		const String &GetName() const;

		bool IsInstanceOf(const Rtti &rtType) const;
		bool IsDerivedFrom(const Rtti &rtType) const;

	private:
		String m_sName;
		const Rtti *m_pBaseType;
	};

	#include "K15_Rtti.inl"
}} //end of K15_Engine::System namespace

#endif //__K15Engine_System_RTTI_h_