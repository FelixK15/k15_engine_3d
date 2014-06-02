/**
 * @file TD_GameObjectFactory.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/28
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

#ifndef _TD_GameObjectFactory_h_
#define _TD_GameObjectFactory_h_

#include "TD_Prerequisities.h"

namespace TowerDefense {
	class GameObjectFactory
	{
	public:
		static GameObject* createSpider();
	};// end of GameObjectFactory class declaration
}// end of TowerDefense namespace

#endif //_TD_GameObjectFactory_h_