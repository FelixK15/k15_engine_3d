#ifndef _K15_OSLayer_EventTypes_h_
#define _K15_OSLayer_EventTypes_h_

// EVENT TYPES
#define K15_APPLICATION_QUIT 100

#define K15_WINDOW_CREATED 200
#define K15_WINDOW_FOCUS_GAINED 201
#define K15_WINDOW_FOCUS_LOST 202
#define K15_WINDOW_MOVED 203

#define K15_KEY_PRESSED 300
#define K15_KEY_RELEASED 301
#define K15_MOUSE_BUTTON_PRESSED 302
#define K15_MOUSE_BUTTON_RELEASED 303
#define K15_MOUSE_MOVED 304
#define K15_WHEEL_MOVED 305
#define K15_CONTROLLER_CONNECTED 306
#define K15_CONTROLLER_DISCONNECTED 307
#define K15_CONTROLLER_BUTTON_PRESSED 308
#define K15_CONTROLLER_BUTTON_RELEASED 309
#define K15_CONTROLLER_DPAD_LEFT 310
#define K15_CONTROLLER_DPAD_RIGHT 311
#define K15_CONTROLLER_DPAD_UP 312
#define K15_CONTROLLER_DPAD_DOWN 313
#define K15_CONTROLLER_START 314
#define K15_CONTROLLER_SELECT 315
#define K15_CONTROLLER_LEFT_SHOULDER_BUTTON 316
#define K15_CONTROLLER_RIGHT_SHOULDER_BUTTON 317
#define K15_LEFT_SHOULDER_TRIGGER 318
#define K15_RIGHT_SHOULDER_TRIGGER 319
#define K15_LEFT_THUMB_X 320
#define K15_LEFT_THUMB_Y 321
#define K15_RIGHT_THUMB_X 322
#define K15_RIGHT_THUMB_Y 323


// EVENT FLAGS
#define K15_WINDOW_EVENT_FLAG 0x001
#define K15_SYSTEM_EVENT_FLAG 0x002
#define K15_INPUT_EVENT_FLAG 0x004

// BOOKKEEPING
#define K15_EVENT_FLAG_COUNT 3
#define K15_EVENT_TYPE_COUNT 11

#define K15_WINDOW_EVENT_ID 0
#define K15_SYSTEM_EVENT_ID 1
#define K15_INPUT_EVENT_ID 2

//CONTROLLER BUTTONS
#define K15_CONTROLLER_FACE_DOWN_BUTTON 1
#define K15_CONTROLLER_FACE_LEFT_BUTTON 2
#define K15_CONTROLLER_FACE_UP_BUTTON 3
#define K15_CONTROLLER_FACE_RIGHT_BUTTON 4

#define K15_EVENTS_PER_BUCKET 128  //12 Kbyte 

#define K15_GET_CONTROLLER_INDEX(event)(short)(event.controllerButton >> 16)

#endif //_K15_OSLayer_EventTypes_h_