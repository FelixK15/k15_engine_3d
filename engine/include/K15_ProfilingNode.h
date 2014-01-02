/**
 * @file K15_ProfileNode.h
 * @author  Felix Klinge <f.klinge@k15games.de>
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

#ifndef _K15Engine_Core_ProfileNode_h_
#define _K15Engine_Core_ProfileNode_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_ProfilingManager.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core { 
	class K15_CORE_API AutoProfilingNode
	{
	public:
		AutoProfilingNode(ProfilingNode* p_Node);
		~AutoProfilingNode();

	private:
		ProfilingNode* m_Node;
	};// end of AutoProfilingNode class declaration
	/*********************************************************************************/
	class K15_CORE_API ProfilingNode : public ProfilingManagerAllocatedObject
	{
	public:
    /*********************************************************************************/
    static const uint32 MaxChildren = 20;
    /*********************************************************************************/

	public:
		ProfilingNode(const String& p_Name);
		virtual ~ProfilingNode();

		ProfilingNode* Parent;
    ProfilingNode* Children[MaxChildren];
		String Name;
    int CountChildren;
    const int ThreadID;
    const int FrameIndex;
    const double StartTime;
		double Time;
		double EndTime;		
	};
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ProfileNode_h_