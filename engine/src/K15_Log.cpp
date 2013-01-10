/**
 * @file K15_Log.cpp
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
 */

#include "K15_Log.h"

using namespace K15_EngineV2;

Log::Log(const String &sName,bool bDebugLog,bool bFileLog)
	: m_sName(sName),
	  m_bDebugLog(bDebugLog)
{
	String sFileName = sName;
	sFileName += ".log";

	if(bFileLog){
		m_pFile = fopen(sFileName.C_Str(),"w");
	}
}

Log::~Log()
{
	if(m_pFile){
		fclose(m_pFile);
	}
}

void Log::LogMessage(const String &sMessage)
{
	if(m_pFile){
		fwrite(sMessage.C_Str(),sizeof(char),sMessage.Size(),m_pFile);
		fwrite("\n",1,1,m_pFile);
	}else{
		// @todo write to console
	}
	
}
