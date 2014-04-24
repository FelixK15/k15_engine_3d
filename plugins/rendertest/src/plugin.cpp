/**
 * @file plugin.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/08
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

#include "plugin.h"
#include "K15_Application.h"
#include "K15_SampleApplicationModule.h"

K15_Engine::Plugins::RenderTest::RenderTestApplicationModule* module = 0;

/*********************************************************************************/
void pluginLoad()
{
	if(!module)
	{
		module = K15_NEW K15_Engine::Plugins::RenderTest::RenderTestApplicationModule();
		g_Application->addApplicationModule(module);
	}
}
/*********************************************************************************/
void pluginUnload()
{
	K15_DELETE module;
	module = 0;
}
/*********************************************************************************/
K15_Engine::Core::ApplicationModuleDescription getDescription()
{
	K15_Engine::Core::ApplicationModuleDescription description;

	description.CompiledWithEngineVersion = K15_ENGINE_VERSION;
	description.Author = "K15 Games";

	return description;
}
/*********************************************************************************/

