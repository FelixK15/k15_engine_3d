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

#ifndef _K15Engine_Core_ProfilingManager_h_
#define _K15Engine_Core_ProfilingManager_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#	include "K15_Singleton.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ProfilingNode.h"

#define K15_PROFILE(profile_name) K15_Engine::Core::ProfilingNode n_##__COUNTER__(_N(profile_name));

namespace K15_Engine { namespace Core { 

 	class ProfilingManager : public Singleton<ProfilingManager>, public ApplicationAllocatedObject							
	{
	public:
		/*********************************************************************************/
		typedef DynamicArray(ProfilingNode*) ProflingNodeList;
		/*********************************************************************************/
	public:
		ProfilingManager();
		~ProfilingManager();

		void addNode(ProfilingNode* p_Node);
		void removeNode(ProfilingNode* p_Node);
		void clear();

		INLINE ProfilingNode* getRootNode();
	private:
		ProflingNodeList m_Nodes;
		ProfilingNode* m_Root;
		ProfilingNode* m_LastNode;
	};// end of ProfilingManager class
#include "K15_ProfilingManager.inl"
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ProfilingManager_h_