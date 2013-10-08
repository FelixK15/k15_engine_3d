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

#ifndef _K15Engine_System_ProfilingManager_h_
#define _K15Engine_System_ProfilingManager_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_ProfilingNode.h"
#include "K15_Singleton.h"

#define K15_PROFILE_APPEND(profile_name) K15_Engine::System::ProfileNode ___p_node___(profile_name,(Enum)K15_Engine::System::ProfileManager::AM_APPEND_LAST_NODE)
#define K15_PROFILE_NEW_BRANCH(profile_name) K15_Engine::System::ProfileNode ___p_node___(profile_name,(Enum)K15_Engine::System::ProfileManager::AM_NEW_BRANCH)

namespace K15_Engine { namespace System { 

 	class ProfilingManager : public Singleton<ProfilingManager>, public ApplicationAllocatedObject
											
	{
	public:
		typedef K15_DynamicArray(ProfilingNode) ProfileNodeList;

		enum eAppendMode
		{
			AM_NEW_BRANCH,
			AM_APPEND_LAST_NODE
		};
	public:
		ProfilingManager();
		~ProfilingManager();

		void addProfileNode(ProfilingNode p_Node);
		void clear();

		ProfilingNode* getRootNode();

	private:
		ProfilingNode* m_Root;
		ProfilingNode* m_LastNode;

		ProfileNodeList m_Nodes;
	};// end of ProfilingManager class
#include "K15_ProfilingManager.inl"
}}//end of K15_Engine::System namespace

#endif //_K15Engine_System_ProfilingManager_h_