#ifndef _K15_Runtime_Config_File_h_
#define _K15_Runtime_Config_File_h_

#include "K15_Prerequisites.h"

struct K15_ConfigFileContext;
uint8 K15_LoadConfigFile(const char* p_ConfigFile, K15_ConfigFileContext* p_ConfigFileContext);

#endif //_K15_Runtime_Config_File_h_