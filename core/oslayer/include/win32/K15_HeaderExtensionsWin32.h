
#ifndef _K15_Header_Extensions_Win32_h_
#define _K15_Header_Extensions_Win32_h_

 #define DIRECTINPUT_VERSION 0x0800	// use directinput 8

#include <mmreg.h>
#include <Dbt.h>
#include <Wbemidl.h>
#include <Xinput.h>
#include <dinput.h>
#include <dsound.h>
#include <io.h>

 #define WIN32_COM_SAFE_RELEASE(ptr) if (ptr){ ptr->Release(); ptr = 0;}

#endif //_K15_Header_Default_Win32_h_
