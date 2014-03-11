/**
 * @file K15_ApplicationLayerOS_Android.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/03/02
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

#include "K15_PrecompiledHeader.h"

#include "K15_RenderWindowBase.h"
#include "K15_RenderTask.h"
#include "K15_RenderProcessBase.h"

#include "Android\K15_ApplicationLayerOS_Android.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const String ApplicationOSLayer_Android::OSName = "Android";
	const String ApplicationOSLayer_Android::PluginExtension = "so";
	/*********************************************************************************/
	
	/*********************************************************************************/
	int32 Android_HandleMotion(android_app* p_App, AInputEvent* p_Event)
	{
		int32 eventAction = AMotionEvent_getAction(p_Event);

		GameEvent* motionEvent = 0;
		
		if(eventAction == AMOTION_EVENT_ACTION_DOWN || eventAction == AMOTION_EVENT_ACTION_UP)
		{

			motionEvent = K15_NEW GameEvent(_EN(onMousePressed))
		}
	}
	/*********************************************************************************/
	int32 Android_HandleKey(android_app* p_App, AInputEvent* p_Event)
	{

	}
	/*********************************************************************************/

	//Callbacks
	/*********************************************************************************/
	int32 AndroidCallback_OnInput(android_app* p_App,AInputEvent* p_Event)
	{
		int32 inputType = AInputEvent_getType(p_Event);
		if(inputType == AINPUT_EVENT_TYPE_KEY)
		{
			_LogDebug("AINPUT_EVENT_TYPE_KEY received.");
			return Android_HandleKey(p_App,p_Event);
		}
		else if(inputType == AINPUT_EVENT_TYPE_MOTION)
		{
			_LogDebug("AINPUT_EVENT_TYPE_MOTION received.");
			return Android_HandleMotion(p_App,p_Event);
		}

		return 0;
	}
	/*********************************************************************************/
	void AndroidCallback_OnCmd(android_app* p_App, int32 p_Cmd)
	{
		Application* app = static_cast<Application*>(p_App->userData);
		if(p_Cmd == APP_CMD_DESTROY)
		{
			_LogDebug("APP_CMD_DESTROY received.");
			app->setRunning(false);
		}
		else if(p_Cmd == APP_CMD_GAINED_FOCUS || p_Cmd == APP_CMD_LOST_FOCUS)
		{
			if(p_Cmd == APP_CMD_GAINED_FOCUS)
			{
				_LogDebug("APP_CMD_GAINED_FOCUS received.");
			}
			else
			{
				_LogDebug("APP_CMD_LOST_FOCUS received.");
			}

			RenderWindowBase* window = 0;
			if((window = app->getRenderWindow()) != 0)
			{
				window->setHasFocus(p_Cmd == APP_CMD_GAINED_FOCUS);
				return;
			}

			_LogError("Could not get render window.");
		}
		else if(p_Cmd == APP_CMD_WINDOW_REDRAW_NEEDED)
		{
			_LogDebug("APP_CMD_WINDOW_REDRAW_NEEDED received.");
			RenderProcessBase* renderProcess = 0;
			if(app->getRenderTask())
			{
				if((renderProcess = app->getRenderTask()->getRenderProcess()) != 0)
				{
					renderProcess->renderSingleFrame();
					return;
				}

				_LogError("Could not get render process.");
			}

			_LogError("Could not get render task.");
		}
		else
		{
			if(p_Cmd == APP_CMD_INPUT_CHANGED) _LogDebug("APP_CMD_INPUT_CHANGED received.");
			if(p_Cmd == APP_CMD_INIT_WINDOW) _LogDebug("APP_CMD_INIT_WINDOW received.");
			if(p_Cmd == APP_CMD_TERM_WINDOW) _LogDebug("APP_CMD_TERM_WINDOW received.");
			if(p_Cmd == APP_CMD_WINDOW_RESIZED) _LogDebug("APP_CMD_WINDOW_RESIZED received.");
			if(p_Cmd == APP_CMD_CONTENT_RECT_CHANGED) _LogDebug("APP_CMD_CONTENT_RECT_CHANGED received.");
			if(p_Cmd == APP_CMD_CONFIG_CHANGED) _LogDebug("APP_CMD_CONFIG_CHANGED received.");
			if(p_Cmd == APP_CMD_LOW_MEMORY) _LogDebug("APP_CMD_LOW_MEMORY received.");
			if(p_Cmd == APP_CMD_START) _LogDebug("APP_CMD_START received.");
			if(p_Cmd == APP_CMD_RESUME) _LogDebug("APP_CMD_RESUME received.");
			if(p_Cmd == APP_CMD_SAVE_STATE) _LogDebug("APP_CMD_SAVE_STATE received.");
			if(p_Cmd == APP_CMD_STOP) _LogDebug("APP_CMD_STOP received.");
			if(p_Cmd == APP_CMD_PAUSE) _LogDebug("APP_CMD_PAUSE received.");
		}
	}
	/*********************************************************************************/

	/*********************************************************************************/
	void* ApplicationOSLayer_Android::os_malloc(uint32 p_Size)
	{
		return malloc(p_Size);
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Android::os_free(void* p_Pointer)
	{
		free(p_Pointer);
	}	
	/*********************************************************************************/
	ApplicationOSLayer_Android::ApplicationOSLayer_Android()
		: m_SensorEventQueue(0),
		m_SensorManager(0)
	{

	}
	/*********************************************************************************/
	ApplicationOSLayer_Android::~ApplicationOSLayer_Android()
	{

	}
	/*********************************************************************************/
	bool ApplicationOSLayer_Android::initialize()
	{
		K15_ASSERT(m_App,"Need to set app instance before initializing android application.");

		if((m_SensorManager = ASensorManager_getInstance()) == 0);
		{
			_LogError("Could not obtain Android Sensor Manager.");
			return false;
		}

		if((m_AccelerometerSensor = 
			(ASensor*)ASensorManager_getDefaultSensor(m_SensorManager,ASENSOR_TYPE_ACCELEROMETER)) == 0)
		{
			_LogError("Could not obtain android accelerometer sensor.");
			return false;
		}
		
		if((m_SensorEventQueue = 
			ASensorManager_createEventQueue(m_SensorManager,m_App->looper,LOOPER_ID_USER,0,0)) == 0)
		{
			_LogError("Could not create accelerometer event queue.");
			return false;
		}

		m_App->userData = g_Application;
		m_App->onInputEvent = 
		//here

		return true;
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Android::shutdown()
	{

	}
	/*********************************************************************************/
	const String& ApplicationOSLayer_Android::getError()
	{
		String err = "Could not retrieve last error from OS.";

		if(!m_Error.empty())
		{
			String err = m_Error;
			m_Error.clear();
		}

		return err;
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Android::getSupportedResolutions(SupportedResolutionSet* p_ResolutionSet) const
	{

	}
	/*********************************************************************************/
	double ApplicationOSLayer_Android::getTime() const
	{

	}
	/*********************************************************************************/
	void ApplicationOSLayer_Android::sleep(double p_TimeInSeconds) const
	{
		unsigned long milliseconds = p_TimeInSeconds * 1000;
		::usleep(milliseconds);
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Android::onPreTick()
	{
		static int ident;
		static int events;
		android_poll_source* source;
		
		while(ALooper_pollAll(0,NULL,&events,(void**)&source))
		{
			if(source)
			{
				source->process(m_App,source);
			}

			if(ident == LOOPER_ID_USER)
			{
				
			}
		}
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Android::onPostTick()
	{

	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace