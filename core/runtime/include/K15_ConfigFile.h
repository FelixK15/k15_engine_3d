#ifndef _K15_Runtime_Config_File_h_
#define _K15_Runtime_Config_File_h_

#include "K15_RuntimePrerequisites.h"
#include "generated/K15_ConfigValueBuffer.h"

/*********************************************************************************/
struct K15_ConfigValue;

typedef void(*K15_ValueChangedCallback)(K15_ConfigValue*);
/*********************************************************************************/
struct K15_ConfigValue
{
	char* name;
	char* value;
	char* category;
};
/*********************************************************************************/
struct K15_ConfigFileContext
{
	char* path;
	K15_ConfigValueStretchBuffer configValues;
};
/*********************************************************************************/

uint8 K15_LoadConfigFile(const char* p_ConfigFile, K15_ConfigFileContext* p_ConfigFileContext);
float K15_GetConfigValueAsFloat(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, float p_DefaultValue = 0.f);
int K15_GetConfigValueAsInt(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, int p_DefaultValue = 0);
bool8 K15_GetConfigValueAsBool(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, bool8 p_DefaultValue = 0);
char* K15_GetConfigValueAsString(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, char* p_DefaultValue = 0);
void K15_GetConfigValueAsResolution(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, int* p_Width, int* p_Height, int p_DefaultWidth = 0, int p_DefaultHeight = 0);
#endif //_K15_Runtime_Config_File_h_