/**
 * @file K15_GpuProgramParameter.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/18
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

#include "K15_GpuProgramParameter.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const GpuProgramParameter::DefaultVariableNameMap GpuProgramParameter::DefaultVariableNames = _createDefaultVariableNameMap();
	const ObjectName GpuProgramParameter::Default::WorldViewProjectionMatrix = _ON(WorldViewProjectionMatrix);
	const ObjectName GpuProgramParameter::Default::InverseWorldViewProjectionMatrix = _ON(InverseWorldViewProjectionMatrix);
	/*********************************************************************************/

	/*********************************************************************************/
	GpuProgramParameter::GpuProgramParameter()
    : m_AutoParameter(false),
    m_Offset(0),
    m_Size(0),
    m_Type(0),
    m_Name(),
    m_AutoName(ObjectName::BLANK),
    m_RegisterIndex(0),
    m_BufferIndex(-1)
	{

	}
	/*********************************************************************************/
	GpuProgramParameter::~GpuProgramParameter()
	{

	}
	/*********************************************************************************/
	GpuProgramParameter::DefaultVariableNameMap GpuProgramParameter::_createDefaultVariableNameMap()
	{
		static DefaultVariableNameMap map;

		map[Default::WorldViewProjectionMatrix] = "g_WorldViewProjMatrix";
		map[Default::InverseWorldViewProjectionMatrix] = "g_InvWorldViewProjMatrix";

		return map;
	}
  /*********************************************************************************/
}}// end of K15_Engine::Core namespace