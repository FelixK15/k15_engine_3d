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

#ifndef _K15Engine_Core_ApplicationModule_h_
#define _K15Engine_Core_ApplicationModule_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Core { 

	class K15_CORE_API ApplicationModule : public ModuleAllocatedObject
	{
	public:
		void onRendererInitialized(const RendererBase* p_Renderer);

		virtual void evaluateSettings(const StringSet& p_ApplicationSettings){}

		virtual void onInitialize(){}
		virtual void onShutdown(){}

		virtual void onPreTick(){}
		virtual void onPreRun(){}
		virtual void onPreRender(/*const RenderContext* p_RenderContext*/){}

		virtual void onPostTick(){}
		virtual void onPostRun(){}
		virtual void onPostRender(/*const RenderContext* p_RenderContext*/){}

	protected:
		ApplicationModule();
		virtual ~ApplicationModule();

		virtual void _onRendererInitialized(const RendererBase* p_Renderer) {};

		bool m_RendererInitialized;
	}; //end of ApplicationModule class
}} //end of K15_Engine::Core namespace

#endif