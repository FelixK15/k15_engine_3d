/**
 * @file ParserMain.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/15
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

#define _CRT_SECURE_NO_WARNINGS //don't throw a "unsafe function" warning when using printf, sprintf, etc. 

#include <string>
#include "LuaPlus.h"

/*********************************************************************************/
void printHelp()
{
	printf("This is a parser for parsing c++ *.h files for K15 Engine specific keywords.\n\n"
		"Configure Options:\n"
		"\t-help\t-\tShows this help text.\n"
		"\t-d\t-\tEnables debug output.\n"
		"\t-i\t-\tIgnores errors. Application will succeed\n\t\t\teven when errors occured.\n\n");
}
/*********************************************************************************/
int main(int argc,char** argv)
{
	static std::string appPath = argv[0];
	std::string::size_type pos = std::string::npos;

	if((pos = appPath.find_last_of('\\')) != std::string::npos)
	{
		appPath = appPath.substr(0,pos+1);
	}

	bool debugOutput = false;
	bool ignoreErrors = false;

	int fileIndex = -1;

	//checking command line arguments
	for(int i = 1;i < argc;++i)
	{
		if(strcmp(argv[i],"-help") == 0)
		{
			printHelp();
			return 0;
		}
		else if(strcmp(argv[i],"-d") == 0)
		{
			debugOutput = true;
		}
		else if(strcmp(argv[i],"-i") == 0)
		{
			ignoreErrors = true;
		}
		else if(strstr(argv[i],".h") != 0)
		{
			fileIndex = i;
		}
	}
	
	//check if last argument is header file
	if(fileIndex == -1)
	{
		printf("No header file (*.h) specified.\n");
		return ignoreErrors ? 0 : -1;
	}

	//create new lua state
	LuaPlus::LuaStateOwner state(true);
	std::string scriptPath = appPath + "lua\\parser.lua";
	if(state->DoFile(scriptPath.c_str()) != 0)
	{
		printf("Could not load script file \"%s\"\nError:%s.",scriptPath.c_str(),state->ToString(-1));
		return ignoreErrors ? 0 : -1;
	}
	
	LuaPlus::LuaFunction<void> parseFunction = state->GetGlobal("parse");
	parseFunction(argv[fileIndex],debugOutput);
	
	return 0;
}
/*********************************************************************************/