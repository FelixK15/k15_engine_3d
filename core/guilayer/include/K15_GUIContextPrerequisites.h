#ifndef _K15_GUILayer_Prerequisites_h_
#define _K15_GUILayer_Prerequisites_h_

#include "K15_Prerequisites.h"
#include "K15_RenderPrerequisites.h"
#include "K15_RuntimePrerequisites.h"
#include "K15_GUIContextConfig.h"

struct K15_GUIContext;
struct K15_GUIElement;

struct K15_ResourceContext;
struct K15_RenderCommandQueue;
struct K15_RenderContext;

#define K15_GUI_CONTEXT_MEMORY_SIZE size_kilobyte(100)
#define K15_GUI_MAX_LAYOUTS 20
#define K15_GUI_MAX_ELEMENTS_PER_LAYOUT 128
#define K15_GUI_MEMORY_BUFFER_COUNT 2
#define K15_GUI_MEMORY_FRONT_BUFFER 0
#define K15_GUI_MEMORY_BACK_BUFFER 1

#define K15_GUI_MAX_BUFFERED_MOUSE_INPUTS 100
#define K15_GUI_MAX_BUFFERED_KEYBOARD_INPUTS 100
#define K15_GUI_MAX_BUFFERED_SYSTEM_EVENTS 100

#define K15_GUI_2D_SQUARE_SAMPLES 20
#endif //_K15_GUILayer_Prerequisites_h_