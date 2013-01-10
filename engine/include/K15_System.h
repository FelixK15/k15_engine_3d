/**
 * @file K15_System.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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
 * The System class is part of the Platform Independence Layer of the engine.
 * It encapsulates calls to the OS like memory allocation, message procession or
 * Hardware queries.
 * 
 */
#pragma once

#ifndef __K15_SYSTEM__
#define __K15_SYSTEM__

//The System object can get obtained using this macro.
//Example:	g_pSystem->Sleep(500);
//			g_pSystem->TimeSinceStart();
#define g_pSystem K15_EngineV2::System::GetInstance()

#include "K15_StdInclude.h"
#include "K15_Singleton.h"
#include "K15_String.h"
#include "K15_GameTime.h"

namespace K15_EngineV2
{
	class Game;
	class Log;

	class K15ENGINE2_API System : public Singleton<System>
	{
	public:
		//Some of the flags that can be OR'd together for the Initialize() function.
		//If you want - for example - a framed fullscreen window you just do
		//@code{.cpp}
		//int flags = System::WF_FRAME | System::WF_FULLSCREEN;
		//g_pSystem->InitializeWindow(/*...*/,flags,/*...*/);
		//@endcode
		enum SystemFlags : unsigned int{
			SF_ENABLE_MEMORY_LOGGING = 0x000002,			/*!<You can turn on memory logging for debugging*/	
			SF_SERIALIZED_HEAP_ALLOCATION = 0x000004,		/*!<You can turn on serialized heap access for multithreaded heap access*/	
		};

		enum WindowFlags : unsigned int{
			WF_FRAME = 0x000002,			/*!< Framed Window */
			WF_AUTOALIGNMENT = 0x000004,	/*!< Class tries to align the window to the current desktop resolution */
			WF_FULLSCREEN = 0x000008		/*!< Window will be created in fullscreen mode */
		};

		enum SystemObjectDescription : unsigned int{
			SOD_WINDOWS_FORM_HANDLE,
		};

		/**
		* Standard Constructor
		*/
		System();

		/**
		* Standard Destructor
		*/
		~System();

		/**
		* Try to initialize the System.
		*
		* @param  iFlags - optional flags that can be OR'd together.See System::SystemFlags for possible flags.
		*
		* @return bool - true if System got initialized and false if there was an error.
		*/
		bool Initialize(U32 iFlags);

		/**
		* Try to create a new window.
		*
		* @param  sTitle - Title of the new window.
		* @param  iFlags - optional flags that can be OR'd together.See System::WindowFlags for possible flags.
		* @param  iPosX - X Position of the new window - will be ignored if flag WF_FULLSCREEN or WF_AUTOALIGNMENT has been set.
		* @param  iPosY - Y Position of the new window - will be ignored if flag WF_FULLSCREEN or WF_AUTOALIGNMENT has been set.
		* @param  iWidth - Width of the new window - will be ignored if flag WF_AUTOALIGNMENT has been set.
		* @param  iHeight - Height of the new window - will be ignored if flag WF_AUTOALIGNMENT has been set.
		*
		* @return bool - true if Window got created and false if there was an error.
		*/
		bool InitializeWindow(const String &sTitle,U32 iFlags,U32 iPosX,U32 iPosY,U32 iWidth,U32 iHeight);

		/**
		* Shuts down the System (This should be the last thing the K15Engine does.)
		*
		*/
		void Shutdown();

		/**
		* Returns the time that has passed since the engine started.
		*
		* @return double - time that has passed since the engine started
		*/
		double TimeSinceStart();

		/** 
		  *	Puts the process to sleep for a certain amount of time.
		  *
		  * @param dTime - time the process will sleep (in ms)
		  */
		void Sleep(double dTime);

		/**
		* This method starts the GameLoop and updates the subsystems frequently.
		* The method also checks for OS events during each frame.
		*
		* It only returns when the game has been shut down.
		*/
		void Run();

		/**
		* Scans the underlying OS for the last error and returns a string if an error happend.
		*
		* @return String - Message that specifies the error.
		*/
		String GetSystemError() const;

		/** 
		  * Returns the current home directory (a directory where you - normally - have write access.
		  *
		  * @returns String - Absolute path of the home directory.
		  */
		String GetHomeDirectory() const;

		/** 
		  * Returns the current directory of the application.
		  *
		  * @returns String - Absolute path of the application directory.
		  */
		String GetApplicationDirectory() const;

		/**
		* Shows a message box with a given message.
		*
		* @param  sMessage - Message of the message box
		* @param  sTitle - Title of the message box
		*/
		void ShowMessageBox(const String& sMessage,const String& sTitle = "K15 Message") const;

		/**
		* Query for specific OS objects. (like windows handles, etc.)
		*
		* @param  eObjectDescription - See SystemObjectDescription Enum for a list of system objects you can query for.
		*
		* @returns void* -	pointer to void that contains the address of the desired OS object 
		*					(or NULL if the OS does not support the object you queried for)
		*/
		void *QuerySystemObject(SystemObjectDescription eObjectDescription) const;

		void SetGame(Game *pGame);

	private:
		void _QueryMemory(size_t *pSystemMemory,size_t *pProcessMemory);

	private:
		Log *m_pEngineLog;
		Game *m_pGame;
		bool m_bFullscreen;
		bool m_bWindowInitialized;
		byte padding[2];
	};
}

#endif //__K15_SYSTEM__