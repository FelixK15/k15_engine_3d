/**
 * @file K15_Material.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_PrecompiledHeader.h"

#include "K15_Material.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	MaterialPass::MaterialPass()
		: m_Programs(),
		m_ProgramBatch(0),
		m_Shininess(0.0f),
		m_Specular(ColorRGBA::White),
		m_Diffuse(ColorRGBA::White),
		m_Ambient(ColorRGBA::White),
		m_DiffuseMap(0),
		m_DiffuseSampler(0),
		m_CullingMode(RendererBase::CM_CW),
		m_FillMode(RendererBase::FM_SOLID),
		m_Transparent(false),
		m_LightningEnabled(true),
		m_BackFaceCullingEnabled(true),
		m_PassEnabled(false)
	{
	
	}
	/*********************************************************************************/
	MaterialPass::~MaterialPass()
	{

	}
	/*********************************************************************************/

	/*********************************************************************************/
	Material::Material()
		: m_Passes()
	{
	
	}
	/*********************************************************************************/
	Material::~Material()
	{
		
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace