/**
 * @file K15_Log.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/17
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
#pragma once

#ifndef __K15_LOG__
#define __K15_LOG__

#include "K15_StdInclude.h"
#include "K15_String.h"

namespace K15_EngineV2
{
	class K15ENGINE2_API Log
	{
	public:
		Log(const String &sName,bool bDebugLog,bool bFileLog);
		~Log();

		void LogMessage(const String &sMessage);

	private:
		String m_sName;
		FILE *m_pFile;
		bool m_bDebugLog;
	};
}

#endif //__K15_LOG__