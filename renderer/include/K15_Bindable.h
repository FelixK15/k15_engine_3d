/**
 * @file K15_Bindable.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#ifndef __K15_BINDABLE__
#define __K15_BINDABLE__

#include "K15_Object.h"

using namespace K15_EngineV2;

namespace K15_Renderer
{
	typedef U32 BindableHandle;
	class Bindable : public Object
	{
		DECLARE_RTTI;

		friend class Renderer;

	public:
		virtual ~Bindable();
		BindableHandle GetHandle() const;

	protected:
		Bindable();

	private:
		BindableHandle m_iHandle;

	};
	#include "../src/K15_Bindable.inl"
	typedef Pointer<Bindable> BindablePtr;
}

#endif //__K15_BINDABLE__