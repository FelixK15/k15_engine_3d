/**
 * @file K15_GpuProgramAutoParameter.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/23
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

#include "K15_GpuProgramAutoParameter.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	GpuProgramAutoParameter::AutoParameterUpdateFunctionMap GpuProgramAutoParameter::m_AutoParameterUpdateFunctionMap = GpuProgramAutoParameter::_createAutoParameterUpdateFunctionMap();
	/*********************************************************************************/

	/*********************************************************************************/
	GpuProgramAutoParameter::AutoParameterSettings::AutoParameterSettings(GpuProgramParameter::UpdateFunc p_UpdateFunction,Enum p_UpdateFrequency,void* p_UserData)
		: UpdateFunction(p_UpdateFunction),
		UpdateFrequency(p_UpdateFrequency),
		UserData(p_UserData)
	{

	}
	/*********************************************************************************/

	/*********************************************************************************/
	bool GpuProgramAutoParameter::checkForAutoParameter(GpuProgramParameter& p_GpuParameter)
	{
		AutoParameterUpdateFunctionMap::iterator iter = m_AutoParameterUpdateFunctionMap.find(p_GpuParameter.getName());

		if(iter != m_AutoParameterUpdateFunctionMap.end())
		{
			_LogDebug("Found auto parameter \"%s\".",p_GpuParameter.getName().c_str());

			p_GpuParameter.setUpdateFunction(iter->second.UpdateFunction);
			p_GpuParameter.setUpdateFrequency(iter->second.UpdateFrequency);
			p_GpuParameter.setUserData(iter->second.UserData);
			p_GpuParameter.setAutoParameter(true);
			return true;
		}

		return false;
	}
	/*********************************************************************************/
	void GpuProgramAutoParameter::addAutoParameter(const String& p_Name,const AutoParameterSettings& p_Settings)
	{
		AutoParameterUpdateFunctionMap::iterator iter = m_AutoParameterUpdateFunctionMap.find(p_Name);

		if(iter != m_AutoParameterUpdateFunctionMap.end())
		{
			_LogError("There's already an auto parameter defined with the name \"%s\".",p_Name.c_str());
		}
		else
		{
			m_AutoParameterUpdateFunctionMap.insert(Pair(String,AutoParameterSettings)(p_Name,p_Settings));
		}
	}
	/*********************************************************************************/
	GpuProgramAutoParameter::AutoParameterUpdateFunctionMap GpuProgramAutoParameter::_createAutoParameterUpdateFunctionMap()
	{
		static AutoParameterUpdateFunctionMap UpdateFunctions;

		UpdateFunctions.insert(Pair(String,AutoParameterSettings)(
			"g_ViewMatrix",AutoParameterSettings(&GpuProgramParameter::UpdateFunctions::UpdateViewMatrix,GpuProgramParameter::UF_PER_CAMERA)));

		UpdateFunctions.insert(Pair(String,AutoParameterSettings)(
			"g_ViewProjMatrix",AutoParameterSettings(&GpuProgramParameter::UpdateFunctions::UpdateViewProjectionMatrix,GpuProgramParameter::UF_PER_CAMERA)));

		UpdateFunctions.insert(Pair(String,AutoParameterSettings)(
			"g_ProjMatrix",AutoParameterSettings(&GpuProgramParameter::UpdateFunctions::UpdateProjectionMatrix,GpuProgramParameter::UF_PER_CAMERA)));

		return UpdateFunctions;
	}
/*********************************************************************************/
}}// end of K15_Engine::Core namespace