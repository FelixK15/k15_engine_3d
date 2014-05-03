/**
 * @file K15_RenderSampleProcess.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/09
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

#include "K15_RenderSampleProcess.h"

#include "K15_Material.h"
#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"
#include "K15_IOUtil.h"
#include "K15_RenderOperation.h"
#include "K15_GpuProgramBatch.h"
#include "K15_ResourceManager.h"
#include "K15_Texture.h"
#include "K15_Image.h"

#include "K15_Mesh.h"
#include "K15_SubMesh.h"

namespace K15_Engine { namespace Plugins { namespace RenderTest {

	/*********************************************************************************/
	RenderSampleProcess::RenderSampleProcess()
	{
 		rop[0] = 0;
		rop[1] = 0;
	}
	/*********************************************************************************/
	RenderSampleProcess::~RenderSampleProcess()
	{

	}
	/*********************************************************************************/
	void RenderSampleProcess::renderSingleFrame()
	{
		if(rop[0] != 0 && rop[1] != 0)
		{
			g_Application->getRenderer()->draw(rop[0]);
			g_Application->getRenderer()->draw(rop[1]);
		}
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Plugins::RenderTest namespace`