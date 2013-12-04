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

#include "K15_PrecompiledHeader.h"

#include "Win32\K15_ApplicationOSLayer_Win32.h"
#include "Win32\K15_RenderWindow_Win32.h"
#include "K15_RenderWindowBase.h"
#include "K15_LogManager.h"
#include "K15_Application.h"
#include "K15_StringUtil.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	const String ApplicationOSLayer_Win32::OSName = "Microsoft Windows";
	const String ApplicationOSLayer_Win32::PluginExtension = "dll";
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
			_LogError("Could not get performance counter frequency. Error:%s",Application::getInstance()->getLastError().c_str());
			return false;
		}

		m_Frequency = double(m_PerformanceCounterFrequency.QuadPart);

		return true;
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Win32::shutdown()
	{

	}
	/*********************************************************************************/
	String ApplicationOSLayer_Win32::getError() const
	{
		String errorMsg;
		char* errorBuffer = 0;
		DWORD lastError = GetLastError();
		DWORD writtenChars = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,0,lastError,MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_UK),(LPSTR)&errorBuffer,0,0);
		if(writtenChars == 0 && !errorBuffer)
		{
			errorBuffer = (char*)alloca(K15_ERROR_BUFFER_SIZE);
			sprintf(errorBuffer,"Could not retrieve last error from OS.");
		}

		errorMsg = errorBuffer;
		
		if(writtenChars)
		{
			LocalFree(errorBuffer);
		}

		return errorMsg;
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Win32::getSupportedResolutions(SupportedResolutionSet* p_ResolutionSet) const
	{
		DEVMODE dm = {0};
		dm.dmSize = sizeof(dm);
		
		for(int i = 0;EnumDisplaySettings(0,i,&dm) != 0;++i)
		{
			Resolution currentResolution = {dm.dmPelsWidth,dm.dmPelsHeight};
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
	void ApplicationOSLayer_Win32::sleep(double p_TimeInSeconds) const
	{
		static DWORD msecs = 0;
		msecs = (DWORD)(p_TimeInSeconds * 100);
		Sleep(msecs);
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Win32::onPreTick()
	{
		static MSG msg;
		static HWND hwnd = (HWND)INVALID_HANDLE_VALUE;
		
		hwnd = ((RenderWindow_Win32*)Application::getInstance()->getRenderWindow())->getHandleWindow();
		
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

      if(msg.message == WM_QUIT)
      {
        Application::getInstance()->setRunning(false);
      }
		}
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Win32::onPostTick()
	{

	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace