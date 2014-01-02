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

#define K15_PROFILE(profile_name) K15_Engine::Core::AutoProfilingNode p_##__COUNTER__(K15_NEW ProfilingNode(profile_name));
#define K15_PROFILE_BLOCK(profile_name,code) { K15_PROFILE(profile_name); code;}

namespace K15_Engine { namespace Core { 

 	class ProfilingManager : public Singleton<ProfilingManager>, public ApplicationAllocatedObject, public PoolAllocator<K15_SIZE_PROFILING_NODE>				
	{
	public:
		/*********************************************************************************/
		typedef List(ProfilingNode*) ProfilingNodeList;
		/*********************************************************************************/
	public:
		ProfilingManager();
		~ProfilingManager();

		void startProfiling(ProfilingNode* p_Node);
		void stopProfiling(ProfilingNode* p_Node);
    void eraseProfilingForFrame(uint32 p_FrameIndex);
		void clear();

		INLINE ProfilingNode* getRootNode();
    INLINE ProfilingNode* getCurrentNode();

  private:
    void _eraseNode_r(ProfilingNode* p_Node);

	private:
		ProfilingNodeList m_Nodes;
		ProfilingNode* m_Root;
		ProfilingNode* m_LastNode;
	};// end of ProfilingManager class
#include "K15_ProfilingManager.inl"
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ProfilingManager_h_