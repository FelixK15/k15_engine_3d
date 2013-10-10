/**
 * @file K15_ApplicationOSLayer_Win32.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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

#include "K15_ApplicationOSLayer_Win32.h"
#include "K15_RenderWindowBase.h"
#include "K15_LogManager.h"
#include "K15_Application.h"
#include "K15_StringUtil.h"

namespace K15_Engine { namespace System { 
	/*********************************************************************************/
	const String ApplicationOSLayer_Win32::OSName = "Microsoft Windows";
	/*********************************************************************************/
	ApplicationOSLayer_Win32::ApplicationOSLayer_Win32()
		: ApplicationOSLayerBase()
	{

	}
	/*********************************************************************************/
	ApplicationOSLayer_Win32::~ApplicationOSLayer_Win32()
	{

	}
	/*********************************************************************************/
	bool ApplicationOSLayer_Win32::initialize()
	{
		if(QueryPerformanceFrequency(&m_PerformanceCounterFrequency) == FALSE)
		{
			_LogError(StringUtil::format("Could not get performance counter frequency. Error:%s",Application::getInstance()->getLastError()));
			return false;
		}

		m_Frequency = double(m_PerformanceCounterFrequency.QuadPart);

		return true;
	}
	/*********************************************************************************/
	String ApplicationOSLayer_Win32::getError() const
	{
		char* errorBuffer = (char*)_malloca(K15_ERROR_BUFFER_SIZE);
		errorBuffer = '\0';
		DWORD lastError = GetLastError();
		DWORD writtenChars = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,0,lastError,1033,errorBuffer,K15_ERROR_BUFFER_SIZE,0);
		if(writtenChars == 0)
		{
			sprintf(errorBuffer,"Could not retrieve last error from OS.");
		}
		return String(errorBuffer);
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Win32::getSupportedResolutions(SupportedResolutionSet* p_ResolutionSet) const
	{
		DEVMODE dm = {0};
		dm.dmSize = sizeof(dm);
		
		for(int i = 0;EnumDisplaySettings(0,i,&dm) != 0;++i)
		{
			RenderWindowBase::Resolution currentResolution = {dm.dmPelsWidth,dm.dmPelsHeight};
			p_ResolutionSet->push_back(currentResolution);
		}
	}
	/*********************************************************************************/
	double ApplicationOSLayer_Win32::getTime() const
	{
		LARGE_INTEGER counts;
		QueryPerformanceCounter(&counts);

		return counts.QuadPart / m_Frequency;
	}
	/*********************************************************************************/
}}//end of K15_Engine::System namespace