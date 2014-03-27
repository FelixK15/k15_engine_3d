/**
 * @file K15_SampleApplicationModule.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/08
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

#ifndef _K15Engine_Plugin_RenderTest_ApplicationModule_h_
#define _K15Engine_Plugin_RenderTest_ApplicationModule_h_

#include "K15_ApplicationModule.h"

namespace K15_Engine { namespace Plugins { namespace RenderTest {
	class RenderTestApplicationModule : public ApplicationModule
	{
	public:
		RenderTestApplicationModule();
		~RenderTestApplicationModule();

		void onInitialize() OVERRIDE;
		void onShutdown() OVERRIDE;
		void onPreTick() OVERRIDE;
		void onPostTick() OVERRIDE;
		
	private:
		void _dumpMemoryStatistics();
	};// end of RenderTestApplicationModule class declaration
}}}// end of K15_Engine::Plugins::RenderTest namespace

#endif //_K15Engine_Plugin_RenderTest_ApplicationModule_h_