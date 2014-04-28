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
#include "K15_RendererBase.h"
#include "K15_CameraComponent.h"
#include "K15_Matrix4.h"
#include "K15_RawData.h"

namespace K15_Engine { namespace Rendering {
  /*********************************************************************************/
  const String GpuProgramParameter::ParameterIdentifierName[GpuProgramParameter::PI_COUNT] = {
    "g_ViewMatrix",     //PI_VIEW_MATRIX
    "g_ProjMatrix",     //PI_PROJECTION_MATRIX
    "g_ModelMatrix",    //PI_MODEL_MATRIX
    "g_ViewProjMatrix" //PI_VIEW_PROJECTION_MATRIX
  }; //ParameterIdentifierName
  /*********************************************************************************/

	/*********************************************************************************/
	GpuProgramParameter::GpuProgramParameter()
		:  m_Offset(0),
		m_Size(0),
		m_Type(0),
		m_Name(),
		m_RegisterIndex(0),
		m_UpdateFrequency(0),
		m_Identifier(0),
		m_BufferIndex(-1),
		m_AutoParameter(false),
		m_Program(0)
	{

	}
	/*********************************************************************************/
	GpuProgramParameter::~GpuProgramParameter()
	{

	}
  /*********************************************************************************/
}}// end of K15_Engine::Core namespace