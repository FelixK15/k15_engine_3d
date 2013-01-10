/**
 * @file K15_UsefulMacro.cpp
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
 */

#ifndef HR
#define HR(x) \
		{ \
			HRESULT hr = (x); \
			if(FAILED(hr)){ \
				DXTraceW(__FILE__,(DWORD)__LINE__,hr,L#x,true); \
			} \
		}
#endif

#ifndef RELEASE_COM
#define RELEASE_COM(x) if(x != NULL){x->Release();}
#endif