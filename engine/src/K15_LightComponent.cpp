/**
 * @file K15_LightComponent.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/04
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

#include "K15_LightComponent.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,LightComponent,GameObjectComponentBase);
	/*********************************************************************************/

	/*********************************************************************************/
	LightComponent::LightComponent()
		: GameObjectComponentBase(_TN(LightComponent)),
		m_LightType(LT_POINT),
		m_Range(0),
		m_DiffuseColor(ColorRGBA::White),
    m_SpecularColor(ColorRGBA::White),
		m_Active(true),
		m_Dirty(false)
	{

	}
	/*********************************************************************************/
	LightComponent::~LightComponent()
	{
		
	}
	/*********************************************************************************/
	void LightComponent::onAddedToRenderQueue(RenderQueue* p_RenderQueue)
	{

	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace