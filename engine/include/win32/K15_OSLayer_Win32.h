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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_ApplicationOSLayer_Win32_h_
#define _K15Engine_Core_ApplicationOSLayer_Win32_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif// K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	class K15_CORE_API OSLayer_Win32
	{
	public:
		/*********************************************************************************/
		static const String OSName;
		static const String PluginExtension;
		typedef DynamicArray(Resolution) SupportedResolutionSet;
		/*********************************************************************************/
	public:
		
		/**
		* Allocate memory with OS specific functions.
		*
		* @param p_Size - size (in bytes) to allocate.
		*
		* @return void* - ptr to allocated memory (0 on error)
		*/
		static void* os_malloc(uint32 p_Size);

		/**
		* Free memory with OS specific functions.
		*
		* @param p_Pointer - Pointer to previously allocated memory.
		*/
		static void os_free(void* p_Pointer);

		/**
		* initialize the OS layer
		*
		*
		* @return bool - true initialization succeeded, false initialization failed (refer to program log for further information)
		*/
		static bool initialize();

		/**
		* shutdown OS layer
		*/
		static void shutdown();
		
		/**
		* get the last error set by the OS
		*
		*
		* @return const String& - Error string
		*/
		static const String& getError();

		/** 
		* get supported resolutions
		*
		* @param p_ResolutionSet - Pointer to a SupportedResolutionSet variable. The function will fill the container.
		*/
		static void getSupportedResolutions(SupportedResolutionSet* p_ResolutionSet);

		/**
		* get current time (1.0 = 1 sec)
		*
		*
		* @return double - current time in second format.
		*/
		static float getTime();
	
		/**
		* Put the calling thread to sleep for a specific amount of time.
		*
		* @param p_TimeInSeconds - seconds the calling thread should idle
		*/
		static void sleep(float p_TimeInSeconds);

		/**
		* callback before the game gets ticked (pumps messages)
		*/
		static void onPreTick();

		/**
		* callback after the game gets ticked
		*/
		static void onPostTick();

	private:
		static LARGE_INTEGER ms_PCF; //Perforamnce Counter Frequency
		static float ms_Frequency;
	};//end of ApplicationOSLayer_Win32 class
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ApplicationOSLayer_Win32_h_