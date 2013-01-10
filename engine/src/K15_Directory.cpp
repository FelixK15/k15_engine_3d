/**
 * @file K15_Directory.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/20
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

#include "K15_Directory.h"
#include "K15_LogManager.h"

using namespace K15_EngineV2;

bool Directory::GetFileList( const char *pDirectory,List<String> &lFilesOut,bool bRecursive,const char *pFilter )
{
// 	String sDirectory = pDirectory;
// 	String sFile;
// 
// 	if(sDirectory.FindLastOf('\\') < (sDirectory.Size() - 1)){
// 		sDirectory += '\\';
// 	}
// 
// 	DIR *pDir = opendir(sDirectory.C_Str());
// 	DIR *pTempDir = NULL;
// 	dirent *pEntry = NULL;
// 
// 	if(!pDir){
// 		WriteDefaultLog(String("Could not open directory. Path: ") + sDirectory);
// 		return false;
// 	}
// 
// 	while(pEntry = readdir(pDir)){
// 		sFile = sDirectory + pEntry->d_name;
// 		
// 		if(sFile.FindLastOf('.') < (sFile.Size() - 1)){
// 			GetFileList(sFile.C_Str(),lFilesOut,bRecursive,pFilter);
// 		}else{
// 			lFilesOut.PushBack(sFile);
// 		}
// 	}
// 	
// 	closedir(pDir);

	return true;
}
