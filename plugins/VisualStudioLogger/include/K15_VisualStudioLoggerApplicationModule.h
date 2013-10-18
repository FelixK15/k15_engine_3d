/**
 * @file K15_VisualStudioLoggerApplicationModule.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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
 */

#include "K15_Prerequisites.h"
#include "K15_ApplicationModule.h"

namespace K15_Engine { namespace Plugins { namespace VisualStudioLogger {
	/*********************************************************************************/
	class VisualStudioLoggerApplicationModule : public System::ApplicationModule
	{
	public:
		bool onInitialize();
	}; //end of VisualStudioLoggerApplicationModule class
	/*********************************************************************************/
}}} //end of K15_Engine::Plugins::VisualStudioLogger namespace