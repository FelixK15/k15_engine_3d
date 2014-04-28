/**
 * @file K15_RenderOperation.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/10
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

#ifndef _K15Engine_Renderer_RenderOperation_h_
#define _K15Engine_Renderer_RenderOperation_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Rendering {
	struct K15_CORE_API RenderOperation : public AllocatedObject<BaseAllocatedObject::AC_RENDEROP_POOL>
	{
		/*********************************************************************************/
		enum eTopology
		{
			T_DOT = 0,
			T_LINE,
			T_TRIANGLE,
			T_TRIANGLE_STRIP,
			T_QUAD,

			T_COUNT
		}; //Topology
		/*********************************************************************************/

    GameObject* gameobject;
		IndexBuffer* indexBuffer;
		VertexBuffer* vertexBuffer;
		Material* material;
		SubMesh* subMesh;
		AlphaState* alphaState;
		Enum topology;
	};// end of RenderOperation struct
}}// end of K15_Engine::Rendering namespace

#endif //