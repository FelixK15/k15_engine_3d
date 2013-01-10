/**
 * @file K15_Importer.h
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
#ifndef __K15_IMPORTER__
#define __K15_IMPORTER__

#include "K15_StdInclude.h"
#include "K15_VertexLayout.h"

namespace K15_Renderer
{
	class Importer
	{
	public:
		static bool ImportOBJ(const char *pFileName,VertexLayoutPtr pVertexLayout,float *&pVertices,U32 *&pIndices,U32 &iVertexCount,U32 &iIndexCount);
		//static bool ImportBlender
		//static bool ImportMaya
		//static bool Import3DSMax
	};
}

#endif //__K15_IMPORTER__