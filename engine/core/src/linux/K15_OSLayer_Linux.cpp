/**
 * @file K15_OSLayer_Linux.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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
 *
 * @section DESCRIPTION
 */

#include "K15_PrecompiledHeader.h"
#include "K15_RenderWindow.h"
#include "K15_Mouse.h"
#include "K15_Keyboard.h"

#ifdef K15_OS_LINUX

#include "linux/K15_OSLayer_Linux.h"

namespace K15_Engine { namespace Core {
    /*********************************************************************************/
    const String OSLayer_Linux::OSName = "Linux";
    const String OSLayer_Linux::PluginExtension = "so";
    /*********************************************************************************/

    /*********************************************************************************/
    timespec resolution;
    timespec timer;
    /*********************************************************************************/

    /*********************************************************************************/
    void* OSLayer_Linux::os_malloc(uint32 p_Size)
    {
        return ::malloc(p_Size);
    }
    /*********************************************************************************/
    void OSLayer_Linux::os_free(void *p_Pointer)
    {
        ::free(p_Pointer);
    }
    /*********************************************************************************/
    bool OSLayer_Linux::initialize()
    {
        clock_getres(CLOCK_MONOTONIC, &resolution);
        return true;
    }
    /*********************************************************************************/
    void OSLayer_Linux::shutdown()
    {

    }
    /*********************************************************************************/
    const String& OSLayer_Linux::getError()
    {
        static String errorMsg;

        errorMsg = strerror(errno);

        if(errorMsg.empty())
        {
            errorMsg = dlerror();
        }

        return errorMsg;
    }
    /*********************************************************************************/
    void OSLayer_Linux::getSupportedResolutions(SupportedResolutionSet *p_ResolutionSet)
    {

    }
    /*********************************************************************************/
    float OSLayer_Linux::getTime()
    {
        //http://stackoverflow.com/questions/3832097/how-to-get-the-current-time-in-native-android-code
        clock_gettime(CLOCK_MONOTONIC, &timer);
        timer.tv_sec /= resolution.tv_sec == 0 ? 1 : resolution.tv_sec;
        timer.tv_nsec /= resolution.tv_nsec == 0 ? 1 : resolution.tv_nsec;
        float milliseconds = 1000.0 * timer.tv_sec + (float) timer.tv_nsec / 1e6;
        return milliseconds / 1000.0;
    }
    /*********************************************************************************/
    void OSLayer_Linux::sleep(float p_TimeInSeconds)
    {
        float microseconds_float = p_TimeInSeconds * 1000000.f;
        ::usleep((unsigned long)microseconds_float);
    }
    /*********************************************************************************/
    void OSLayer_Linux::onPreTick()
    {
        XEvent event;
        while(XPending(RenderWindowImpl::ms_Display))
        {
            XNextEvent(RenderWindowImpl::ms_Display, &event);
			switch(event.type)
			{
			case ResizeRequest:
				_onResize(event);
				break;

			case ClientMessage
				if(event.xclient.data.l[0] == RenderWindowImpl::ms_DeleteWindowID)
				{
					g_Application->setRunning(false);
				}
				break;
			}
        }
    }
    /*********************************************************************************/
    void OSLayer_Linux::onPostTick()
    {

    }
    /*********************************************************************************/
    void OSLayer_Linux::_onResize(const XEvent& p_Event)
    {
        ResizeEventArguments args;

        Resolution newResolution;
        newResolution.height = p_Event.xresizerequest.height;
        newResolution.width = p_Event.xresizerequest.width;

        args.oldWidth = RenderWindow::getWidth(); args.oldHeight = RenderWindow::getHeight();
        args.newWidth = p_Event.xresizerequest.width; args.newHeight = p_Event.xresizerequest.height;

        RenderWindow::setResolution(args.newWidth, args.newHeight);

        GameEvent* resolutionChangedEvent = K15_NEW GameEvent(RenderWindow::EventResolutionChanged,
                                                              (void*)&args, sizeof(ResizeEventArguments));

        g_EventManager->addEventToQueue(resolutionChangedEvent);
    }
    /*********************************************************************************/
}} //end of K15_Engine::Core namespace

#endif //K15_OS_LINUX
