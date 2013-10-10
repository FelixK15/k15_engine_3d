/**
 * @file K15_ApplicationOSLayer_Win32.h
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
	String ApplicationOSLayer_Win32::getError()
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
}}//end of K15_Engine::System namespace