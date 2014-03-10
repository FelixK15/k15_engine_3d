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

#include "Android\K15_ApplicationLayerOS_Android.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const String ApplicationOSLayer_Android::OSName = "Android";
	const String ApplicationOSLayer_Android::PluginExtension = "so";
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

		return true;
	}
	/*********************************************************************************/
	void ApplicationOSLayer_Android::shutdown()
	{

	}
	/*********************************************************************************/
	const String& ApplicationOSLayer_Android::getError() const
	{

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