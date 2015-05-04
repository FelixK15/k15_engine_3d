#include "K15_Prerequisites.h"

struct K15_OSLayerContext;

K15_CORE_API uint8 K15_InitializeCore(K15_OSLayerContext* p_OSLayer);
K15_CORE_API void K15_ShutdownCore();
K15_CORE_API void K15_TickCore(float p_DeltaTime);