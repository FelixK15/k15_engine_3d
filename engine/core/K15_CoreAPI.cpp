#include "K15_PrecompiledHeader.h"

#include <K15_OSLayer_OSContext.h>

#include "K15_Application.h"
#include "K15_CoreAPI.h"

static K15_Engine::Application* application = 0;

/*********************************************************************************/
uint8 K15_InitializeCore(K15_OSLayerContext* p_OSLayer)
{
	application = K15_NEW K15_Engine::Application();

	application->initialize(p_OSLayer->commandLineArgCount, p_OSLayer->commandLineArgs);

	return K15_SUCCESS;
}
/*********************************************************************************/
void K15_ShutdownCore()
{
	application->shutdown();

	K15_DELETE application;
	application = 0;
}
/*********************************************************************************/
void K15_TickCore(float p_DeltaTime)
{
	application->tick(p_DeltaTime);
}
/*********************************************************************************/