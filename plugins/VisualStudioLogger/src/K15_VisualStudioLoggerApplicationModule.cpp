/**
 * @file K15_VisualStudioLoggerApplicationModule.cpp
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
 */

#include "K15_VisualStudioLoggerApplicationModule.h"

#include "K15_LogManager.h"
#include "K15_VisualStudioLog.h"

namespace K15_Engine { namespace Plugins { namespace VisualStudioLogger {
	/*********************************************************************************/
	bool VisualStudioLoggerApplicationModule::onInitialize()
	{
		g_LogManager->addLog(K15_NEW VisualStudioLog(),true);

		return true;
	}
	/*********************************************************************************/
}}} //end of K15_Engine::Plugins::VisualStudioLogger namespace