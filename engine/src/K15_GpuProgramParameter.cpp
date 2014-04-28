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
/*#include "K15_Matrix4.h"*/
#include "K15_RawData.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	GpuProgramParameter::GpuProgramParameter()
		:  m_Offset(0),
		m_Size(0),
		m_Type(0),
		m_Name(),
		m_RegisterIndex(0),
		m_UpdateFrequency(0),
		m_UpdateFunc(0),
		m_BufferIndex(-1),
		m_AutoParameter(false),
		m_Program(0)
	{

	}
	/*********************************************************************************/
	GpuProgramParameter::GpuProgramParameter(const GpuProgramParameter& p_Other)
		: m_Offset(p_Other.m_Offset),
		m_Size(p_Other.m_Size),
		m_Type(p_Other.m_Type),
		m_Name(p_Other.m_Name),
		m_RegisterIndex(p_Other.m_RegisterIndex),
		m_UpdateFrequency(p_Other.m_UpdateFrequency),
		m_UpdateFunc(p_Other.m_UpdateFunc),
		m_BufferIndex(p_Other.m_BufferIndex),
		m_AutoParameter(p_Other.m_AutoParameter),
		m_Program(p_Other.m_Program)
	{

	}
	/*********************************************************************************/
	GpuProgramParameter::~GpuProgramParameter()
	{

	}
	/*********************************************************************************/
	void GpuProgramParameter::update(RawData& p_Data)
	{
		if(!m_UpdateFunc.isValid())
		{
			_LogWarning_NoSpam("GpuProgramParameter \"%s\" has an invalid update function.",m_Name.c_str());
		}
		else
		{
			p_Data = m_UpdateFunc(*this,m_UserData);
		}
	}
	/*********************************************************************************/

	/*********************************************************************************/
	RawData GpuProgramParameter::UpdateFunctions::UpdateProjectionMatrix(const GpuProgramParameter& p_Param, void* p_UserData)
	{
		static RawData projMatrixData;
		RendererBase* renderer = g_Application->getRenderer();

		if(renderer && renderer->getActiveCamera())
		{
			projMatrixData.data = (byte*)&renderer->getActiveCamera()->getProjectionMatrix();
		}
		else
		{
			/*projMatrixData.data = (byte*)&Matrix4::Identity;*/
		}

		projMatrixData.size = sizeof(Matrix4);
		
		return projMatrixData;
	}
	/*********************************************************************************/
	RawData GpuProgramParameter::UpdateFunctions::UpdateViewMatrix(const GpuProgramParameter& p_Param, void* p_UserData)
	{
		static RawData viewMatrixData;
		RendererBase* renderer = g_Application->getRenderer();

		if(renderer && renderer->getActiveCamera())
		{
			viewMatrixData.data = (byte*)&renderer->getActiveCamera()->getViewMatrix();
		}
		else
		{
			/*viewMatrixData.data = (byte*)&Matrix4::Identity;*/
		}

		viewMatrixData.size = sizeof(Matrix4);

		return viewMatrixData;
	}
	/*********************************************************************************/
	RawData GpuProgramParameter::UpdateFunctions::UpdateViewProjectionMatrix(const GpuProgramParameter& p_Param, void* p_UserData)
	{
		static Matrix4 viewMatrix;
		static Matrix4 projectionMatrix;
		static Matrix4 viewProjectionMatrix;
		static RawData viewProjectionMatrixData;
		RendererBase* renderer = g_Application->getRenderer();

		if(renderer && renderer->getActiveCamera())
		{
			viewMatrix = renderer->getActiveCamera()->getViewMatrix();
			projectionMatrix = renderer->getActiveCamera()->getProjectionMatrix();
		}
		else
		{
// 			viewMatrix = Matrix4::Identity;
// 			projectionMatrix = Matrix4::Identity;
		}

		viewProjectionMatrix = viewMatrix * projectionMatrix;
		viewProjectionMatrixData.data = (byte*)&viewProjectionMatrixData;
		viewProjectionMatrixData.size = sizeof(Matrix4);

		return viewProjectionMatrixData;
	}
  /*********************************************************************************/
	void GpuProgramParameter::setUpdateFunction(UpdateFunc p_UpdateFunc)
	{
		K15_ASSERT(p_UpdateFunc.isValid(),
			StringUtil::format("Update function for GpuProgramParameter \"%s\" is invalid.",m_Name.c_str()));

		m_UpdateFunc = p_UpdateFunc;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace