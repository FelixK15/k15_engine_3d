/**
 * @file ParserMain.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/18
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
#include "lua.hpp"

/*********************************************************************************/
int main(int argc,char** argv)
{
  lua_State* state = lua_open();
  luaL_openlibs(state);

  std::string scriptPath = ".\\lua\\main.lua";
  if(luaL_dofile(state,scriptPath.c_str()) != 0)
  {
    printf("Could not load script file \"%s\"\nError:%s.",scriptPath.c_str(),lua_tostring(state,-1));
    return -1;
  }

  lua_getglobal(state,"main");

  lua_createtable(state,0,0);

  int tableIndex = lua_gettop(state);

	//checking command line arguments
	for(int i = 0;i < argc;++i)
	{
    lua_pushnumber(state,i+1);
    lua_pushstring(state,argv[i]);
    lua_settable(state,tableIndex);
	}

  if(lua_pcall(state,1,0,0) != 0)
  {
    const char* errorMsg = lua_tostring(state,-1);
    printf(errorMsg);

    return -1;
  }

  lua_close(state);

	return 0;
}
/*********************************************************************************/