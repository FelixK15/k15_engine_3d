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
#include "linux/K15_OSLayer_Linux.h"

namespace K15_Engine { namespace Core {
    /*********************************************************************************/
    const String OSLayer_Linux::OSName = "Linux";
    const String OSLayer_Linux::PluginExtension = ".so";
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
        struct timespec res;
        clock_gettime(CLOCK_REALTIME, &res);
        return res.tv_sec + (float) res.tv_nsec / 1e6;
    }
    /*********************************************************************************/
    void OSLayer_Linux::sleep(float p_TimeInSeconds)
    {
        unsigned long microseconds = p_TimeInSeconds * 1000000;
        ::usleep(microseconds);
    }
    /*********************************************************************************/
    void OSLayer_Linux::onPreTick()
    {

    }
    /*********************************************************************************/
    void OSLayer_Linux::onPostTick()
    {

    }
    /*********************************************************************************/
}} //end of K15_Engine::Core namespace
