/**
 * @file K15_GpuProgramAutoParameter.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Rendering_GpuProgramAutoParameter_h_
#define _K15Engine_Rendering_GpuProgramAutoParameter_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_GpuProgramParameter.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API GpuProgramAutoParameter
	{
	public:
		/*********************************************************************************/
		struct K15_CORE_API AutoParameterSettings
		{
			GpuProgramParameter::UpdateFunc UpdateFunction;
			Enum UpdateFrequency;
			void* UserData;

			AutoParameterSettings(GpuProgramParameter::UpdateFunc p_UpdateFunction,Enum p_UpdateFrequency,void* p_UserData = 0);
		};
		/*********************************************************************************/

		/*********************************************************************************/
		typedef HashMap(String,AutoParameterSettings) AutoParameterUpdateFunctionMap;
		/*********************************************************************************/

	public:
		static bool checkForAutoParameter(GpuProgramParameter& p_GpuParameter);
		static void addAutoParameter(const String& p_Name,const AutoParameterSettings& p_Settings);

	private:
		static AutoParameterUpdateFunctionMap _createAutoParameterUpdateFunctionMap();

	private:
		static AutoParameterUpdateFunctionMap m_AutoParameterUpdateFunctionMap;
	}; //GpuProgramAutoParameter class declaration
}} // end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_GpuProgramAutoParameter_h_