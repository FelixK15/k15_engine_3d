#include "plugin.h"

#include "K15_Application.h"
#include "TD_GameModule.h"

TowerDefense::GameModule* module = 0;

/*********************************************************************************/
void pluginLoad()
{
	module = K15_NEW TowerDefense::GameModule();
	g_Application->addApplicationModule(module);
}
/*********************************************************************************/
void pluginUnload()
{
	K15_DELETE module;
}
/*********************************************************************************/
ApplicationModuleDescription getDescription()
{
	ApplicationModuleDescription desc;
	desc.Author = "K15 Games";
	desc.CompiledWithEngineVersion = K15_ENGINE_VERSION;
	desc.MajorVersion = 1;
	desc.MinorVersion = 0;

	return desc;
}
/*********************************************************************************/