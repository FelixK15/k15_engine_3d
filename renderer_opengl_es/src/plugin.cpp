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

#if defined K15_OS_ANDROID
#	include "GLES2\K15_GLES2_Renderer.h"
#endif //K15_OS_ANDROID

#include "K15_Application.h"
#include "K15_RenderTask.h"
#include "K15_RenderProcessBase.h"

extern "C"
{
/*********************************************************************************/
void pluginLoad()
{
#ifdef K15_OS_ANDROID
	g_Application->setRenderer(K15_NEW K15_Engine::Rendering::GLES2::Renderer());
#endif //K15_OS_ANDROID
}
/*********************************************************************************/
void pluginUnload()
{
	K15_DELETE g_Application->getRenderer();
}
/*********************************************************************************/
K15_Engine::Core::ApplicationModuleDescription getDescription()
{
	K15_Engine::Core::ApplicationModuleDescription desc;

	desc.Author = "K15 Games";
	desc.CompiledWithEngineVersion = K15_ENGINE_VERSION;
	desc.MajorVersion = 1;
	desc.MinorVersion = 0;
	desc.PluginFlagBitMask = 0;

	return desc;
}
/*********************************************************************************/
};
