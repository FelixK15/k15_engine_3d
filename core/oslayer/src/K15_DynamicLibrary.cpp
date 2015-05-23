#include "K15_DynamicLibrary.h"
#include "K15_DefaultCLibraries.h"
#include "K15_OSContext.h"

#include "generated/K15_DynamicLibraryStretchBuffer.cpp"

/*********************************************************************************/
K15_DynamicLibrary* K15_LoadDynamicLibrary(const char* p_LibraryName)
{
	K15_ASSERT_TEXT(p_LibraryName, "Library Name is NULL.");

	K15_OSContext* context = K15_GetOSLayerContext();
	K15_DynamicLibraryStretchBuffer* dynamicLibraryStretchBuffer = &context->system.dynamicLibraries;
	K15_DynamicLibrary* dynamicLibrary = (K15_DynamicLibrary*)K15_OS_MALLOC(sizeof(K15_DynamicLibrary));

	uint8 result = context->system.loadDynamicLibrary(context, dynamicLibrary, p_LibraryName);

	if (result != K15_SUCCESS)
	{
		K15_OS_FREE(dynamicLibrary);
		dynamicLibrary = 0;
	}
	else
	{
		size_t libNameLength = strlen(p_LibraryName);
		char* libNameBuffer = (char*)K15_OS_MALLOC(libNameLength + 1);
		libNameBuffer[libNameLength] = 0;
		memcpy(libNameBuffer, p_LibraryName, libNameLength);

		dynamicLibrary->name = libNameBuffer;

		K15_PushDynamicLibrary(dynamicLibraryStretchBuffer, dynamicLibrary);
	}

	return dynamicLibrary;
}
/*********************************************************************************/