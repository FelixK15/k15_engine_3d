/**
 * @file K15_ProfileManager.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/10
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

#pragma once

#ifndef __K15_PROFILEMANAGER__
#define __K15_PROFILEMANAGER__

#include "K15_StdInclude.h"
#include "K15_Singleton.h"

#include "K15_ProfileNode.h"
#include "K15_NTree.h"

#define g_pProfileManager K15_EngineV2::ProfileManager::GetInstance()

namespace K15_EngineV2
{
	class ProfileManager : public Singleton<ProfileManager>
	{
	public:
		ProfileManager();
		~ProfileManager();

		void AddProfileNode(const char *pName,double dTimeExecuted);
	private:
		NTree<ProfileNode,20> m_trProfileTree;
	};
}

#endif //__K15_PROFILEMANAGER__