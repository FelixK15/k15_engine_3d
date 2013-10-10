/**
 * @file K15_ApplicationModule.h
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
 *
 * 
 */

#ifndef _K15Engine_System_ApplicationModule_h_
#define _K15Engine_System_ApplicationModule_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace System { 

	class ApplicationModule
	{
  public:
    virtual void evaluateSettings(const StringSet& p_ApplicationSettings){}

    virtual Task* createTask(){return 0;}
    //virtual RenderTask* createRenderTask(){return 0;} 
    virtual bool onInitialize(){}
    virtual void onShutdown(){}

    virtual void onPreTick(){}
    virtual void onPreRun(){}
    virtual void onPreRender(/*const RenderContext* p_RenderContext*/){}

    virtual void onPostTick(){}
    virtual void onPostRun(){}
    virtual void onPostRender(/*const RenderContext* p_RenderContext*/){}
	}; //end of ApplicationModule class
}} //end of K15_Engine::System namespace

#endif