/**
 * @file plugin.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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

/*********************************************************************************/
K15_Engine::System::ApplicationModuleDescription getDescription()
{
	K15_Engine::System::ApplicationModuleDescription desc;

	desc.Author = "Felix Klinge";
	desc.CompiledWithEngineVersion = K15_ENGINE_VERSION;
	desc.MajorVersion = 1;
	desc.MinorVersion = 0;
	desc.PluginFlagBitMask = 0;

	return desc;
}
/*********************************************************************************/
K15_Engine::System::ApplicationModule* getModule()
{
	return new K15_Engine::Plugins::VisualStudioLogger::VisualStudioLoggerApplicationModule();
}
/*********************************************************************************/