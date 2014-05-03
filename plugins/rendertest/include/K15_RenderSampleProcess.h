/**
 * @file K15_RenderSampleProcess.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/09
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

#ifndef _K15Engine_Plugin_RenderTest_RenderSampleProcess_h_
#define _K15Engine_Plugin_RenderTest_RenderSampleProcess_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_RenderProcessBase.h"

namespace K15_Engine { namespace Plugins { namespace RenderTest {
	class RenderSampleProcess : public RenderProcessBase
	{
	public:
		RenderSampleProcess();
		~RenderSampleProcess();

		virtual void renderSingleFrame();

		RenderOperation* rop[2];
	}; //K15_RenderSampleProcess class declaration
}}} // end of K15_Engine::Plugins::RenderTest namespace

#endif //_K15Engine_Plugin_RenderTest_ApplicationModule_h_